#pragma once
#include "core/Entity.h"

#include <algorithm>
#include <cassert>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>

namespace Engine::Core {

// ── ComponentPool ─────────────────────────────────────────────────────────────
// A tightly-packed array of one component type.
// Uses a sparse index (entity → dense slot) for O(1) lookup.
// Iteration over the dense array is cache-friendly.

template<typename T>
class ComponentPool {
public:
    void Insert(EntityID id, T component) {
        assert(!Has(id) && "Entity already has this component");
        m_Sparse[id] = static_cast<uint32_t>(m_Dense.size());
        m_Entities.push_back(id);
        m_Dense.push_back(std::move(component));
    }

    void Remove(EntityID id) {
        assert(Has(id) && "Entity does not have this component");
        auto slot      = m_Sparse[id];
        auto lastSlot  = static_cast<uint32_t>(m_Dense.size() - 1);

        // Swap-erase to keep the dense array packed
        if (slot != lastSlot) {
            m_Dense[slot]    = std::move(m_Dense[lastSlot]);
            m_Entities[slot] = m_Entities[lastSlot];
            m_Sparse[m_Entities[slot]] = slot;
        }
        m_Dense.pop_back();
        m_Entities.pop_back();
        m_Sparse.erase(id);
    }

    T& Get(EntityID id) {
        assert(Has(id));
        return m_Dense[m_Sparse.at(id)];
    }

    const T& Get(EntityID id) const {
        assert(Has(id));
        return m_Dense[m_Sparse.at(id)];
    }

    bool Has(EntityID id) const {
        return m_Sparse.count(id) != 0;
    }

    // Range-for support over all components (dense, cache-friendly)
    auto begin() { return m_Dense.begin(); }
    auto end()   { return m_Dense.end(); }
    auto begin() const { return m_Dense.cbegin(); }
    auto end()   const { return m_Dense.cend(); }
    const std::vector<EntityID>& Entities() const { return m_Entities; }

private:
    std::vector<T>        m_Dense;
    std::vector<EntityID> m_Entities;
    std::unordered_map<EntityID, uint32_t> m_Sparse;
};

// ── Registry ──────────────────────────────────────────────────────────────────
// Central store for all entities and their components.
// Systems iterate components; they never store entity IDs themselves.

class Registry {
public:
    // ── Entity lifecycle ──────────────────────────────────────────────────────
    EntityID Create() {
        EntityID id = ++m_NextID;
        m_Alive.push_back(id);
        return id;
    }

    void Destroy(EntityID id) {
        // Remove from alive list
        m_Alive.erase(std::remove(m_Alive.begin(), m_Alive.end(), id),
                      m_Alive.end());
        // Remove from every pool that knows about it
        for (auto& [type, pool] : m_Pools)
            pool->TryRemove(id);
    }

    // ── Component access ──────────────────────────────────────────────────────
    template<typename T, typename... Args>
    T& Emplace(EntityID id, Args&&... args) {
        auto& pool = GetOrCreate<T>();
        pool.Insert(id, T{std::forward<Args>(args)...});
        return pool.Get(id);
    }

    template<typename T>
    void Remove(EntityID id) {
        GetOrCreate<T>().Remove(id);
    }

    template<typename T>
    T& Get(EntityID id) { return GetOrCreate<T>().Get(id); }

    template<typename T>
    const T& Get(EntityID id) const {
        return const_cast<Registry*>(this)->GetOrCreate<T>().Get(id);
    }

    template<typename T>
    bool Has(EntityID id) const {
        auto it = m_Pools.find(typeid(T));
        if (it == m_Pools.end()) return false;
        return static_cast<TypedPool<T>*>(it->second.get())->pool.Has(id);
    }

    // ── View: iterate all entities that have component T ─────────────────────
    template<typename T>
    ComponentPool<T>& View() { return GetOrCreate<T>(); }

    const std::vector<EntityID>& AllEntities() const { return m_Alive; }

private:
    // Type-erased pool base so we can call TryRemove without knowing T
    struct PoolBase {
        virtual ~PoolBase() = default;
        virtual void TryRemove(EntityID id) = 0;
    };

    template<typename T>
    struct TypedPool : PoolBase {
        ComponentPool<T> pool;
        void TryRemove(EntityID id) override {
            if (pool.Has(id)) pool.Remove(id);
        }
    };

    template<typename T>
    ComponentPool<T>& GetOrCreate() {
        auto key = std::type_index(typeid(T));
        auto it  = m_Pools.find(key);
        if (it == m_Pools.end()) {
            auto [ins, _] = m_Pools.emplace(key, std::make_unique<TypedPool<T>>());
            it = ins;
        }
        return static_cast<TypedPool<T>*>(it->second.get())->pool;
    }

    EntityID m_NextID = 0;
    std::vector<EntityID> m_Alive;
    std::unordered_map<std::type_index, std::unique_ptr<PoolBase>> m_Pools;
};

} // namespace Engine::Core
