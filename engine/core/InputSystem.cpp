#include "core/InputSystem.h"

namespace Engine::Core {

// ── Static storage ─────────────────────────────────────────────────────────
std::array<bool, InputSystem::KEY_COUNT>    InputSystem::s_CurrentKeys    = {};
std::array<bool, InputSystem::KEY_COUNT>    InputSystem::s_PreviousKeys   = {};
std::array<bool, InputSystem::BUTTON_COUNT> InputSystem::s_CurrentButtons = {};
std::array<bool, InputSystem::BUTTON_COUNT> InputSystem::s_PreviousButtons= {};
double InputSystem::s_MouseX     = 0.0;
double InputSystem::s_MouseY     = 0.0;
double InputSystem::s_PrevMouseX = 0.0;
double InputSystem::s_PrevMouseY = 0.0;
double InputSystem::s_ScrollY    = 0.0;

// ── Init ────────────────────────────────────────────────────────────────────
void InputSystem::Init(GLFWwindow* window) {
    glfwSetKeyCallback        (window, OnKey);
    glfwSetMouseButtonCallback(window, OnMouse);
    glfwSetScrollCallback     (window, OnScroll);
    glfwSetCursorPosCallback  (window, OnCursor);

    // Seed initial cursor position so first delta is 0
    glfwGetCursorPos(window, &s_MouseX, &s_MouseY);
    s_PrevMouseX = s_MouseX;
    s_PrevMouseY = s_MouseY;
}

// ── BeginFrame ──────────────────────────────────────────────────────────────
void InputSystem::BeginFrame() {
    s_PreviousKeys     = s_CurrentKeys;
    s_PreviousButtons  = s_CurrentButtons;
    s_PrevMouseX       = s_MouseX;
    s_PrevMouseY       = s_MouseY;
    s_ScrollY          = 0.0;  // cleared each frame; accumulates via callback
}

// ── Keyboard queries ─────────────────────────────────────────────────────────
bool InputSystem::IsKeyDown    (int key) { return s_CurrentKeys[key]; }
bool InputSystem::IsKeyPressed (int key) { return  s_CurrentKeys[key] && !s_PreviousKeys[key]; }
bool InputSystem::IsKeyReleased(int key) { return !s_CurrentKeys[key] &&  s_PreviousKeys[key]; }

// ── Mouse queries ─────────────────────────────────────────────────────────────
bool   InputSystem::IsMouseDown (int btn) { return s_CurrentButtons[btn]; }
double InputSystem::MouseX()      { return s_MouseX; }
double InputSystem::MouseY()      { return s_MouseY; }
double InputSystem::MouseDeltaX() { return s_MouseX - s_PrevMouseX; }
double InputSystem::MouseDeltaY() { return s_MouseY - s_PrevMouseY; }
double InputSystem::ScrollDelta() { return s_ScrollY; }

// ── GLFW callbacks ──────────────────────────────────────────────────────────
void InputSystem::OnKey(GLFWwindow*, int key, int, int action, int) {
    if (key < 0 || key >= KEY_COUNT) return;
    if (action == GLFW_PRESS)   s_CurrentKeys[key] = true;
    if (action == GLFW_RELEASE) s_CurrentKeys[key] = false;
}

void InputSystem::OnMouse(GLFWwindow*, int button, int action, int) {
    if (button < 0 || button >= BUTTON_COUNT) return;
    s_CurrentButtons[button] = (action == GLFW_PRESS);
}

void InputSystem::OnScroll(GLFWwindow*, double, double yOff) {
    s_ScrollY += yOff;  // accumulate within a frame
}

void InputSystem::OnCursor(GLFWwindow*, double x, double y) {
    s_MouseX = x;
    s_MouseY = y;
}

} // namespace Engine::Core
