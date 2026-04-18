#include "platform/Window.h"
#include "core/Registry.h"
#include "core/Camera.h"
#include "core/InputSystem.h"
#include "render/frontend/Renderer.h"

#include "components/RenderComponents.h"

#include <chrono>
#include <cstdio>

// ── Convenience aliases ───────────────────────────────────────────────────────
using namespace Engine;
using Clock     = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Seconds   = std::chrono::duration<float>;

// ── Constants ─────────────────────────────────────────────────────────────────
static constexpr int   WINDOW_W     = 1280;
static constexpr int   WINDOW_H     = 720;
static constexpr float CAMERA_SPEED = 5.0f;   // units per second
static constexpr float MOUSE_SENS   = 0.1f;   // degrees per pixel

// ── Systems ───────────────────────────────────────────────────────────────────
// Each system is a free function: it iterates registry components and
// does one job. No system holds state — state lives in components.

static void InputSystem_Update(Core::Camera& camera, float dt) {
    // -- Translation
    if (Core::InputSystem::IsKeyDown(GLFW_KEY_W))
        camera.MoveForward( CAMERA_SPEED * dt);
    if (Core::InputSystem::IsKeyDown(GLFW_KEY_S))
        camera.MoveForward(-CAMERA_SPEED * dt);
    if (Core::InputSystem::IsKeyDown(GLFW_KEY_A))
        camera.MoveRight  (-CAMERA_SPEED * dt);
    if (Core::InputSystem::IsKeyDown(GLFW_KEY_D))
        camera.MoveRight  ( CAMERA_SPEED * dt);
    if (Core::InputSystem::IsKeyDown(GLFW_KEY_SPACE))
        camera.MoveUp     ( CAMERA_SPEED * dt);
    if (Core::InputSystem::IsKeyDown(GLFW_KEY_LEFT_SHIFT))
        camera.MoveUp     (-CAMERA_SPEED * dt);

    // -- Rotation (right mouse button held = look around)
    if (Core::InputSystem::IsMouseDown(GLFW_MOUSE_BUTTON_RIGHT)) {
        float dx = static_cast<float>(Core::InputSystem::MouseDeltaX()) * MOUSE_SENS;
        float dy = static_cast<float>(Core::InputSystem::MouseDeltaY()) * MOUSE_SENS;
        camera.Rotate(dx, -dy); // invert Y so moving mouse up looks up
    }
}

static void RenderSubmit_System(Core::Registry& registry,
                                Render::Renderer& renderer)
{
    // Iterate every entity that has both a transform and a visible renderable.
    // Systems never call GL directly — they only call Renderer::Submit().
    auto& transforms  = registry.View<Game::TransformComponent>();
    auto& renderables = registry.View<Game::RenderComponent>();

    for (auto id : transforms.Entities()) {
        if (!registry.Has<Game::RenderComponent>(id)) continue;

        auto& rc = registry.Get<Game::RenderComponent>(id);
        if (!rc.Visible) continue;

        auto& tc = registry.Get<Game::TransformComponent>(id);
        renderer.Submit(rc.MeshHandle, rc.MaterialHandle, tc.Matrix());
    }
}

// ── Entry point ───────────────────────────────────────────────────────────────
int main() {
    // ── Window & GL context ───────────────────────────────────────────────────
    Platform::Window window("OpenGL Engine", WINDOW_W, WINDOW_H);

    // ── Input ─────────────────────────────────────────────────────────────────
    Core::InputSystem::Init(window.NativeHandle());

    // ── Renderer ──────────────────────────────────────────────────────────────
    Render::Renderer renderer;
    renderer.Init();
    renderer.SetClearColor(0.12f, 0.12f, 0.15f, 1.0f);

    // ── Camera ────────────────────────────────────────────────────────────────
    float aspect = static_cast<float>(WINDOW_W) / static_cast<float>(WINDOW_H);
    Core::Camera camera(60.0f, aspect);

    // ── ECS registry ──────────────────────────────────────────────────────────
    Core::Registry registry;

    // -- Example entity (swap handle IDs for real AssetManager IDs later)
    {
        auto id = registry.Create();
        registry.Emplace<Game::TagComponent>      (id, Game::TagComponent{"Cube"});
        registry.Emplace<Game::TransformComponent>(id);
        registry.Emplace<Game::RenderComponent>   (id, Game::RenderComponent{
            .MeshHandle     = 1,
            .MaterialHandle = 1,
            .TintColor      = {1.0f, 0.8f, 0.6f, 1.0f},
        });
    }

    // ── Delta time ────────────────────────────────────────────────────────────
    TimePoint lastTime = Clock::now();

    // ── Main loop ─────────────────────────────────────────────────────────────
    while (!window.ShouldClose()) {
        // 1. TIME
        TimePoint now = Clock::now();
        float dt = std::chrono::duration_cast<Seconds>(now - lastTime).count();
        lastTime = now;
        // Clamp to 100ms so a breakpoint or stutter doesn't send physics flying
        if (dt > 0.1f) dt = 0.1f;

        // 2. INPUT (before PollEvents so callbacks fire into fresh state)
        Core::InputSystem::BeginFrame();
        window.PollEvents();

        if (Core::InputSystem::IsKeyPressed(GLFW_KEY_ESCAPE))
            break;

        // 3. SIMULATE
        InputSystem_Update(camera, dt);
        camera.Update();

        // PhysicsSystem::Update(registry, dt);
        // AISystem::Update(registry, dt);
        // AnimationSystem::Update(registry, dt);

        // 4. BUILD RENDER QUEUE
        renderer.BeginScene(camera);
        RenderSubmit_System(registry, renderer);

        // 5. DRAW
        renderer.EndScene();

        // 6. PRESENT
        window.SwapBuffers();
    }

    return 0;
}
