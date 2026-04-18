#pragma once
#include "render/opengl/GLBuffer.h"
#include <memory>
#include <vector>

namespace Engine::Render {

// ── GLVertexArray (VAO) ───────────────────────────────────────────────────────
// Wraps an OpenGL Vertex Array Object.
// Add vertex buffers and an index buffer, then just Bind() before drawing.

class GLVertexArray {
public:
    GLVertexArray();
    ~GLVertexArray();

    GLVertexArray(const GLVertexArray&)            = delete;
    GLVertexArray& operator=(const GLVertexArray&) = delete;

    void Bind()   const;
    void Unbind() const;

    // Attaches a vertex buffer and sets up attribute pointers from its layout.
    // The buffer's layout MUST be set before calling this.
    void AddVertexBuffer(std::shared_ptr<VertexBuffer> vbo);

    // Attaches an index buffer (EBO). Also binds it to this VAO.
    void SetIndexBuffer(std::shared_ptr<IndexBuffer> ibo);

    const std::vector<std::shared_ptr<VertexBuffer>>& VertexBuffers() const { return m_VertexBuffers; }
    const std::shared_ptr<IndexBuffer>&               GetIndexBuffer() const { return m_IndexBuffer; }

    uint32_t IndexCount() const;

private:
    uint32_t m_RendererID    = 0;
    uint32_t m_AttribIndex   = 0;   // next glVertexAttribPointer slot

    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer>               m_IndexBuffer;
};

} // namespace Engine::Render
