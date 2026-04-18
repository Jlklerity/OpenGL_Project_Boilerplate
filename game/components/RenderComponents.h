#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <string>

namespace Game {

// ── TransformComponent ────────────────────────────────────────────────────────
// Separate from RenderComponent so the physics/AI systems can own it
// without depending on any render types.
struct TransformComponent {
    glm::vec3 Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f}; // Euler angles in degrees
    glm::vec3 Scale    = {1.0f, 1.0f, 1.0f};

    // Build the model matrix on demand (cheap, called once per Submit)
    glm::mat4 Matrix() const;
};

// ── RenderComponent ───────────────────────────────────────────────────────────
// Pure data — no GL types, no pointers.
// Handles are resolved by AssetManager at draw time.
struct RenderComponent {
    uint32_t  MeshHandle     = 0;   // ID from AssetManager
    uint32_t  MaterialHandle = 0;   // ID from AssetManager
    glm::vec4 TintColor      = {1.0f, 1.0f, 1.0f, 1.0f};
    bool      CastsShadow    = true;
    bool      Visible        = true;
};

// ── TagComponent ──────────────────────────────────────────────────────────────
// Cheap string identity for debug tools, serialisation, and editor picking.
struct TagComponent {
    std::string Tag = "Entity";
};

} // namespace Game
