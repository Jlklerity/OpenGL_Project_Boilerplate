#pragma once
#include <cstddef>
#include <stdint.h>

namespace Engine::Render {
    // Thin wrapper. Knows about OpenGL concepts but doesn't dictate game logic.
    class VertexBuffer {
    public:
        VertexBuffer(const void* data, uint32_t size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
        
    private:
        uint32_t m_RendererID; // GLuint
    };
}