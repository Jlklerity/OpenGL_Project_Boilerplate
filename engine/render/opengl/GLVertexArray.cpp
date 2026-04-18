#include "render/opengl/GLVertexArray.h"
#include <glad/glad.h>
#include <cassert>

namespace Engine::Render {

static GLenum ShaderTypeToGL(ShaderDataType t) {
    switch (t) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4: return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:   return GL_INT;
        case ShaderDataType::Bool:   return GL_BOOL;
        default:                     return GL_NONE;
    }
}

GLVertexArray::GLVertexArray()  { glGenVertexArrays(1, &m_RendererID); }
GLVertexArray::~GLVertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

void GLVertexArray::Bind()   const { glBindVertexArray(m_RendererID); }
void GLVertexArray::Unbind() const { glBindVertexArray(0); }

void GLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vbo) {
    assert(!vbo->Layout().Elements().empty() &&
           "VertexBuffer has no layout — call SetLayout() before AddVertexBuffer()");

    glBindVertexArray(m_RendererID);
    vbo->Bind();

    const auto& layout = vbo->Layout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(m_AttribIndex);
        glVertexAttribPointer(
            m_AttribIndex,
            static_cast<GLint>(ShaderDataTypeCount(element.Type)),
            ShaderTypeToGL(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE,
            static_cast<GLsizei>(layout.Stride()),
            reinterpret_cast<const void*>(static_cast<uintptr_t>(element.Offset))
        );
        ++m_AttribIndex;
    }

    m_VertexBuffers.push_back(std::move(vbo));
}

void GLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> ibo) {
    glBindVertexArray(m_RendererID);
    ibo->Bind();
    m_IndexBuffer = std::move(ibo);
}

uint32_t GLVertexArray::IndexCount() const {
    return m_IndexBuffer ? m_IndexBuffer->Count() : 0;
}

} // namespace Engine::Render
