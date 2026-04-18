#include "platform/Window.h"
#include "render/frontend/Renderer.h"
#include "core/Entity.h" // Hypothetical ECS

using namespace Engine;

int main() {
    Platform::Window window("OpenGL Engine", 1280, 720);
    Render::Renderer::Init();

    // Setup ECS & Assets...

    while (!window.ShouldClose()) {
        // 1. INPUT
        window.PollEvents();
        // InputSystem::Update();

        // 2. SIMULATE
        // PhysicsSystem::Update(deltaTime);
        // AISystem::Update(deltaTime);

        // 3. BUILD RENDER QUEUE
        Render::Renderer::BeginScene(/* Camera */);
        
        // RenderSubmissionSystem logic:
        // for each entity with Transform & RenderComponent {
        //     Render::Renderer::Submit(mesh, material, transform);
        // }

        // 4. RENDER
        Render::Renderer::EndScene(); // Executes the draw calls backend

        // 5. PRESENT
        window.SwapBuffers();
    }

    return 0;
}