#pragma once
#include <cstdint>
#include <string>

namespace Engine::Render {

enum class TextureFilter { Nearest, Linear, LinearMipmapLinear };
enum class TextureWrap   { Repeat, ClampToEdge, MirroredRepeat };

struct TextureSpec {
    TextureFilter MinFilter = TextureFilter::LinearMipmapLinear;
    TextureFilter MagFilter = TextureFilter::Linear;
    TextureWrap   WrapS     = TextureWrap::Repeat;
    TextureWrap   WrapT     = TextureWrap::Repeat;
    bool          GenMipmaps = true;
};

// ── GLTexture ─────────────────────────────────────────────────────────────────
// Wraps a 2D OpenGL texture.
// Load from file (requires stb_image, bundled via FetchContent) or from raw RGBA.

class GLTexture {
public:
    // Load from an image file on disk (PNG, JPG, BMP, TGA, …)
    explicit GLTexture(const std::string& path,
                       const TextureSpec& spec = TextureSpec{});

    // Create from raw RGBA8 pixel data
    GLTexture(uint32_t width, uint32_t height,
              const void* rgbaData,
              const TextureSpec& spec = TextureSpec{});

    ~GLTexture();

    GLTexture(const GLTexture&)            = delete;
    GLTexture& operator=(const GLTexture&) = delete;

    // Bind to a texture unit (slot 0..15)
    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    uint32_t Width()      const { return m_Width; }
    uint32_t Height()     const { return m_Height; }
    uint32_t RendererID() const { return m_RendererID; }

private:
    void Upload(const void* data, const TextureSpec& spec);
    static uint32_t ToGLFilter(TextureFilter f);
    static uint32_t ToGLWrap  (TextureWrap w);

    uint32_t m_RendererID = 0;
    uint32_t m_Width      = 0;
    uint32_t m_Height     = 0;
};

} // namespace Engine::Render
