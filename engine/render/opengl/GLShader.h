#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Engine::Render {

// ── GLShader ──────────────────────────────────────────────────────────────────
// Compiles and links a GLSL vertex + fragment shader pair.
// Uniform locations are cached on first use to avoid repeated GL calls.

class GLShader {
public:
    // Construct from raw GLSL source strings
    GLShader(const std::string& vertexSrc, const std::string& fragmentSrc);

    // Construct by loading .glsl files from disk
    static GLShader FromFiles(const std::string& vertexPath,
                              const std::string& fragmentPath);

    ~GLShader();

    GLShader(const GLShader&)            = delete;
    GLShader& operator=(const GLShader&) = delete;

    void Bind()   const;
    void Unbind() const;

    // ── Uniform setters ───────────────────────────────────────────────────────
    void SetInt  (const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec2 (const std::string& name, const glm::vec2& v);
    void SetVec3 (const std::string& name, const glm::vec3& v);
    void SetVec4 (const std::string& name, const glm::vec4& v);
    void SetMat3 (const std::string& name, const glm::mat3& m);
    void SetMat4 (const std::string& name, const glm::mat4& m);

    uint32_t ID() const { return m_RendererID; }

private:
    static uint32_t CompileStage(uint32_t glType, const std::string& source);
    static void     LinkProgram (uint32_t program);
    static std::string ReadFile (const std::string& path);

    int GetUniformLocation(const std::string& name);

    uint32_t m_RendererID = 0;
    mutable std::unordered_map<std::string, int> m_UniformCache;
};

} // namespace Engine::Render
