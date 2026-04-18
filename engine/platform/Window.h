#pragma once
#include <string>

struct GLFWwindow;   // forward-declare; keeps GLFW out of every includer

namespace Engine::Platform {

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    // Non-copyable
    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    void PollEvents();
    void SwapBuffers();
    bool ShouldClose() const;

    int  Width()  const { return m_Width; }
    int  Height() const { return m_Height; }
    GLFWwindow* NativeHandle() const { return m_Window; }

private:
    GLFWwindow* m_Window = nullptr;
    int         m_Width  = 0;
    int         m_Height = 0;
};

} // namespace Engine::Platform
