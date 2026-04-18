#include "render/opengl/GLShader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace Engine::Render {

// ── Construction ──────────────────────────────────────────────────────────────
GLShader::GLShader(const std::string& vertSrc, const std::string& fragSrc) {
    uint32_t vert = CompileStage(GL_VERTEX_SHADER,   vertSrc);
    uint32_t frag = CompileStage(GL_FRAGMENT_SHADER, fragSrc);

    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vert);
    glAttachShader(m_RendererID, frag);
    LinkProgram(m_RendererID);

    glDetachShader(m_RendererID, vert);
    glDetachShader(m_RendererID, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);
}

GLShader GLShader::FromFiles(const std::string& vertPath, const std::string& fragPath) {
    return GLShader(ReadFile(vertPath), ReadFile(fragPath));
}

GLShader::~GLShader() {
    glDeleteProgram(m_RendererID);
}

// ── Bind / Unbind ─────────────────────────────────────────────────────────────
void GLShader::Bind()   const { glUseProgram(m_RendererID); }
void GLShader::Unbind() const { glUseProgram(0); }

// ── Uniforms ──────────────────────────────────────────────────────────────────
int GLShader::GetUniformLocation(const std::string& name) {
    auto it = m_UniformCache.find(name);
    if (it != m_UniformCache.end()) return it->second;

    int loc = glGetUniformLocation(m_RendererID, name.c_str());
    if (loc == -1)
        fprintf(stderr, "[GLShader] Warning: uniform '%s' not found\n", name.c_str());

    m_UniformCache[name] = loc;
    return loc;
}

void GLShader::SetInt  (const std::string& n, int v)              { glUniform1i (GetUniformLocation(n), v); }
void GLShader::SetFloat(const std::string& n, float v)            { glUniform1f (GetUniformLocation(n), v); }
void GLShader::SetVec2 (const std::string& n, const glm::vec2& v) { glUniform2fv(GetUniformLocation(n), 1, glm::value_ptr(v)); }
void GLShader::SetVec3 (const std::string& n, const glm::vec3& v) { glUniform3fv(GetUniformLocation(n), 1, glm::value_ptr(v)); }
void GLShader::SetVec4 (const std::string& n, const glm::vec4& v) { glUniform4fv(GetUniformLocation(n), 1, glm::value_ptr(v)); }
void GLShader::SetMat3 (const std::string& n, const glm::mat3& m) { glUniformMatrix3fv(GetUniformLocation(n), 1, GL_FALSE, glm::value_ptr(m)); }
void GLShader::SetMat4 (const std::string& n, const glm::mat4& m) { glUniformMatrix4fv(GetUniformLocation(n), 1, GL_FALSE, glm::value_ptr(m)); }

// ── Private helpers ───────────────────────────────────────────────────────────
uint32_t GLShader::CompileStage(uint32_t glType, const std::string& source) {
    uint32_t id = glCreateShader(glType);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        int logLen = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen);
        glGetShaderInfoLog(id, logLen, nullptr, log.data());
        glDeleteShader(id);
        throw std::runtime_error(
            std::string("[GLShader] Compile error:\n") + log.data());
    }
    return id;
}

void GLShader::LinkProgram(uint32_t program) {
    glLinkProgram(program);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        int logLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> log(logLen);
        glGetProgramInfoLog(program, logLen, nullptr, log.data());
        throw std::runtime_error(
            std::string("[GLShader] Link error:\n") + log.data());
    }
}

std::string GLShader::ReadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("[GLShader] Cannot open file: " + path);

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

} // namespace Engine::Render
