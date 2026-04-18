#pragma once
#include "render/frontend/RenderQueue.h"
#include "core/Camera.h"

// The frontend knows NOTHING about OpenGL types.
// No GLBuffer.h include here — that stays in the .cpp.

namespace Engine::Render {

// ── Renderer ──────────────────────────────────────────────────────────────────
// Owns the render queue for one frame.
// Game systems call Submit(); the backend executes in EndScene().
//
// One Renderer instance = one render pass. Create additional instances
// for shadow maps, reflections, or post-processing targets.

class Renderer {
public:
    Renderer();
    ~Renderer();

    // Non-copyable (owns GL state)
    Renderer(const Renderer&)            = delete;
    Renderer& operator=(const Renderer&) = delete;

    // Called once after the GL context exists
    void Init();

    // ── Per-frame interface ───────────────────────────────────────────────────
    // Call at the start of a frame with scene-wide data
    void BeginScene(const Core::Camera& camera);

    // Game/systems call this — never raw GL
    void Submit(uint32_t meshID, uint32_t materialID, const glm::mat4& transform);

    // Sorts draw calls (by material to minimise state changes), then draws
    void EndScene();

    // ── State ─────────────────────────────────────────────────────────────────
    void SetClearColor(float r, float g, float b, float a = 1.0f);

private:
    void ExecuteDrawCalls();
    void SortByMaterial();   // reduces GL state-change overhead at scale

    FrameData m_Frame;
    bool      m_Initialized = false;
};

} // namespace Engine::Render
