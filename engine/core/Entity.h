#pragma once
#include <cstdint>

namespace Engine::Core {

// ── Entity ────────────────────────────────────────────────────────────────────
// An entity is just a 32-bit ID. The Registry owns all component data;
// entities themselves carry no state.

using EntityID = uint32_t;
inline constexpr EntityID NullEntity = 0;

} // namespace Engine::Core
