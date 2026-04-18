#pragma once
#include <string>

// Forward declarations to keep GLFW out of the header
struct GLFWwindow; 

namespace Engine::Platform {
    class Window {
    public:
        Window(const std::string& title, int width, int height);
        ~Window();

        void PollEvents();
        void SwapBuffers();
        bool ShouldClose() const;

    private:
        GLFWwindow* m_Window;
    };
}