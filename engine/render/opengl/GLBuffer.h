#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <utility>

namespace Engine::Render {

// ── BufferElement / BufferLayout ──────────────────────────────────────────────
// Describes how vertex attributes are packed inside a buffer.
// GLVertexArray uses this to call glVertexAttribPointer automatically.

enum class ShaderDataType { Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Bool };

uint32_t ShaderDataTypeSize(ShaderDataType type);
uint32_t ShaderDataTypeCount(ShaderDataType type); // component count

struct BufferElement {
    std::string    Name;
    ShaderDataType Type;
    uint32_t       Size;
    uint32_t       Offset  = 0;
    bool           Normalized = false;

    BufferElement(ShaderDataType t, std::string name, bool normalized = false)
        : Name(std::move(name)), Type(t),
          Size(ShaderDataTypeSize(t)), Normalized(normalized) {}
};

class BufferLayout {
public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    uint32_t Stride() const { return m_Stride; }
    const std::vector<BufferElement>& Elements() const { return m_Elements; }

    auto begin() { return m_Elements.begin(); }
    auto end()   { return m_Elements.end(); }
    auto begin() const { return m_Elements.cbegin(); }
    auto end()   const { return m_Elements.cend(); }

private:
    void CalculateOffsets();
    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

// ── VertexBuffer ──────────────────────────────────────────────────────────────
class VertexBuffer {
public:
    // Static geometry (GL_STATIC_DRAW)
    VertexBuffer(const void* data, uint32_t sizeBytes);
    // Dynamic buffer: allocate size, fill later with Upload()
    explicit VertexBuffer(uint32_t sizeBytes);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&)            = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    void Bind()   const;
    void Unbind() const;

    // Upload new data (only for dynamic buffers)
    void Upload(const void* data, uint32_t sizeBytes);

    void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
    const BufferLayout& Layout() const         { return m_Layout; }

private:
    uint32_t     m_RendererID = 0;
    BufferLayout m_Layout;
};

// ── IndexBuffer ───────────────────────────────────────────────────────────────
class IndexBuffer {
public:
    IndexBuffer(const uint32_t* indices, uint32_t count);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&)            = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    void Bind()   const;
    void Unbind() const;

    uint32_t Count() const { return m_Count; }

private:
    uint32_t m_RendererID = 0;
    uint32_t m_Count      = 0;
};

} // namespace Engine::Render
