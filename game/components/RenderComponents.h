#pragma once
#include <stdint.h>
#include <glm/glm.hpp>

namespace Game {
    // Pure data. Reusable meshes and materials.
    struct RenderComponent {
        uint32_t MeshHandle;      // ID from AssetManager
        uint32_t MaterialHandle;  // ID from AssetManager
        glm::vec4 TintColor;
    };
}