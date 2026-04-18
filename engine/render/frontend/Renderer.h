#pragma once
#include "render/opengl/GLBuffer.h" // Backend includes are fine here

namespace Engine::Render {
    // Collects renderables, builds passes, sorts materials.
    class Renderer {
    public:
        static void Init();
        static void BeginScene(/* Camera data */);
        static void EndScene();

        // Game/Systems submit to this, they do NOT call OpenGL directly
        static void Submit(uint32_t meshID, uint32_t materialID, const glm::mat4& transform);
    };
}