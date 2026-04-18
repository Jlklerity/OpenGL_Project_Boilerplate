#include "render/opengl/GLBuffer.h"
#include <glad/glad.h>

namespace Engine::Render {

// ── ShaderDataType helpers ─────────────────────────────────────────────────
uint32_t ShaderDataTypeSize(ShaderDataType t) {
    switch (t) {
        case ShaderDataType::Float:  return 4;
        case ShaderDataType::Float2: return 4 * 2;
        case ShaderDataType::Float3: return 4 * 3;
        case ShaderDataType::Float4: return 4 * 4;
        case ShaderDataType::Int:    return 4;
        case ShaderDataType::Int2:   return 4 * 2;
        case ShaderDataType::Int3:   return 4 * 3;
        case ShaderDataType::Int4:   return 4 * 4;
        case ShaderDataType::Bool:   return 1;
        default:                     return 0;
    }
}

uint32_t ShaderDataTypeCount(ShaderDataType t) {
    switch (t) {
        case ShaderDataType::Float:  return 1;
        case ShaderDataType::Float2: return 2;
        case ShaderDataType::Float3: return 3;
        case ShaderDataType::Float4: return 4;
        case ShaderDataType::Int:    return 1;
        case ShaderDataType::Int2:   return 2;
        case ShaderDataType::Int3:   return 3;
        case ShaderDataType::Int4:   return 4;
        case ShaderDataType::Bool:   return 1;
        default:                     return 0;
    }
}

// ── BufferLayout ──────────────────────────────────────────────────────────
BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : m_Elements(elements)
{
    CalculateOffsets();
}

void BufferLayout::CalculateOffsets() {
    uint32_t offset = 0;
    for (auto& e : m_Elements) {
        e.Offset  = offset;
        offset   += e.Size;
    }
    m_Stride = offset;
}

// ── VertexBuffer ──────────────────────────────────────────────────────────
VertexBuffer::VertexBuffer(const void* data, uint32_t sizeBytes) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, sizeBytes, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(uint32_t sizeBytes) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, sizeBytes, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind()   const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }
void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::Upload(const void* data, uint32_t sizeBytes) {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeBytes, data);
}

// ── IndexBuffer ───────────────────────────────────────────────────────────
IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count)
    : m_Count(count)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 count * sizeof(uint32_t),
                 indices,
                 GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind()   const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID); }
void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

} // namespace Engine::Render
