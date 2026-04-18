#pragma once
#include <GLFW/glfw3.h>
#include <array>

namespace Engine::Core {

// ── InputSystem ───────────────────────────────────────────────────────────────
// Poll-based input. Call BeginFrame() once per tick (before PollEvents),
// then query IsKeyDown / IsKeyPressed / mouse state during the frame.
//
// "Pressed" = true for exactly one frame when the key transitions down.
// "Down"    = true every frame the key is held.

class InputSystem {
public:
    // Attach GLFW callbacks to a window. Call once after window creation.
    static void Init(GLFWwindow* window);

    // Call at the START of each frame, before glfwPollEvents().
    static void BeginFrame();

    // ── Keyboard ──────────────────────────────────────────────────────────────
    // key is a GLFW_KEY_* constant (e.g. GLFW_KEY_W)
    static bool IsKeyDown    (int key);
    static bool IsKeyPressed (int key);  // true only on first frame of press
    static bool IsKeyReleased(int key);  // true only on first frame of release

    // ── Mouse ─────────────────────────────────────────────────────────────────
    static bool  IsMouseDown (int button);  // GLFW_MOUSE_BUTTON_LEFT etc.
    static double MouseX();
    static double MouseY();
    static double MouseDeltaX();   // movement since last frame
    static double MouseDeltaY();
    static double ScrollDelta();   // accumulated scroll this frame

private:
    // GLFW callbacks (registered in Init)
    static void OnKey   (GLFWwindow*, int key, int, int action, int);
    static void OnMouse (GLFWwindow*, int button, int action, int);
    static void OnScroll(GLFWwindow*, double xOff, double yOff);
    static void OnCursor(GLFWwindow*, double x, double y);

    static constexpr int KEY_COUNT    = GLFW_KEY_LAST + 1;
    static constexpr int BUTTON_COUNT = GLFW_MOUSE_BUTTON_LAST + 1;

    static std::array<bool, KEY_COUNT>    s_CurrentKeys;
    static std::array<bool, KEY_COUNT>    s_PreviousKeys;
    static std::array<bool, BUTTON_COUNT> s_CurrentButtons;
    static std::array<bool, BUTTON_COUNT> s_PreviousButtons;

    static double s_MouseX,  s_MouseY;
    static double s_PrevMouseX, s_PrevMouseY;
    static double s_ScrollY;   // reset each BeginFrame
};

} // namespace Engine::Core
