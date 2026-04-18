#include "platform/Window.h"

#include <glad/glad.h>      // must come before GLFW
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <cstdio>
#include <string>

namespace Engine::Platform {

// ── GLFW error callback ───────────────────────────────────────────────────────
static void GLFWErrorCallback(int error, const char* description) {
    // In a real engine, route this through a Logger system.
    // For now, stderr is fine — it's always available.
    fprintf(stderr, "[GLFW Error %d] %s\n", error, description);
}

// ── Constructor ───────────────────────────────────────────────────────────────
Window::Window(const std::string& title, int width, int height)
    : m_Width(width), m_Height(height)
{
    glfwSetErrorCallback(GLFWErrorCallback);

    if (!glfwInit())
        throw std::runtime_error("Window: glfwInit() failed");

    // Request an OpenGL 4.1 Core Profile context.
    // Drop to 3.3 if you need broader hardware support.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // required on macOS
#endif

    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        throw std::runtime_error("Window: glfwCreateWindow() failed");
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1); // enable VSync

    // GLAD loads every GL function pointer; must happen after MakeContextCurrent.
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
        throw std::runtime_error("Window: GLAD failed to load OpenGL functions");
    }
}

// ── Destructor ────────────────────────────────────────────────────────────────
Window::~Window() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    glfwTerminate();
}

void Window::PollEvents()   { glfwPollEvents(); }
void Window::SwapBuffers()  { glfwSwapBuffers(m_Window); }
bool Window::ShouldClose() const { return glfwWindowShouldClose(m_Window); }

} // namespace Engine::Platform
