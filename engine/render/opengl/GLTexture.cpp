#include "render/opengl/GLTexture.h"
#include <glad/glad.h>
#include <stdexcept>

// stb_image — header-only image loader bundled via FetchContent in CMake.
// Define the implementation exactly once (here) across the whole project.
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Engine::Render {

// ── File constructor ──────────────────────────────────────────────────────────
GLTexture::GLTexture(const std::string& path, const TextureSpec& spec) {
    int w, h, channels;
    stbi_set_flip_vertically_on_load(true); // OpenGL expects Y=0 at bottom

    stbi_uc* data = stbi_load(path.c_str(), &w, &h, &channels, STBI_rgb_alpha);
    if (!data)
        throw std::runtime_error("[GLTexture] Failed to load: " + path +
                                 "\n  Reason: " + stbi_failure_reason());

    m_Width  = static_cast<uint32_t>(w);
    m_Height = static_cast<uint32_t>(h);
    Upload(data, spec);
    stbi_image_free(data);
}

// ── Raw RGBA constructor ──────────────────────────────────────────────────────
GLTexture::GLTexture(uint32_t width, uint32_t height,
                     const void* rgbaData, const TextureSpec& spec)
    : m_Width(width), m_Height(height)
{
    Upload(rgbaData, spec);
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &m_RendererID);
}

// ── Bind ─────────────────────────────────────────────────────────────────────
void GLTexture::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void GLTexture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ── Private ───────────────────────────────────────────────────────────────────
void GLTexture::Upload(const void* data, const TextureSpec& spec) {
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGLWrap(spec.WrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGLWrap(spec.WrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLFilter(spec.MinFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGLFilter(spec.MagFilter));

    glTexImage2D(GL_TEXTURE_2D,
                 0,              // mip level
                 GL_RGBA8,       // internal format
                 static_cast<GLsizei>(m_Width),
                 static_cast<GLsizei>(m_Height),
                 0,              // border (must be 0)
                 GL_RGBA,        // source format
                 GL_UNSIGNED_BYTE,
                 data);

    if (spec.GenMipmaps)
        glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t GLTexture::ToGLFilter(TextureFilter f) {
    switch (f) {
        case TextureFilter::Nearest:            return GL_NEAREST;
        case TextureFilter::Linear:             return GL_LINEAR;
        case TextureFilter::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
        default:                                return GL_LINEAR;
    }
}

uint32_t GLTexture::ToGLWrap(TextureWrap w) {
    switch (w) {
        case TextureWrap::Repeat:         return GL_REPEAT;
        case TextureWrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
        default:                          return GL_REPEAT;
    }
}

} // namespace Engine::Render
