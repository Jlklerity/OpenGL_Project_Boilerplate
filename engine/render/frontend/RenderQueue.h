#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

namespace Engine::Render {

// ── DrawCall ──────────────────────────────────────────────────────────────────
// A single renderable submitted by game code.
// The frontend accumulates these; the backend executes them.
// All fields are opaque handles — no raw GL types.

struct DrawCall {
    uint32_t  MeshID;       // registered in AssetManager
    uint32_t  MaterialID;   // registered in AssetManager
    glm::mat4 Transform;
};

// ── FrameData ─────────────────────────────────────────────────────────────────
// Everything the backend needs to render one frame.
// Built by Renderer::BeginScene / Submit, consumed by Renderer::EndScene.

struct FrameData {
    glm::mat4            ViewProjection;
    glm::vec3            CameraPosition;
    std::vector<DrawCall> DrawCalls;

    void Clear() {
        DrawCalls.clear();
    }
};

} // namespace Engine::Render
