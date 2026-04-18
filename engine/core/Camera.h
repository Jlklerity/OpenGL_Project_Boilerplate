#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Core {

// ── Camera ────────────────────────────────────────────────────────────────────
// FPS-style perspective camera.
// Call Update() each frame to rebuild the view matrix from position + angles.

class Camera {
public:
    Camera(float fovDegrees, float aspectRatio, float nearZ = 0.1f, float farZ = 1000.0f);

    // Move relative to the camera's own axes
    void MoveForward(float amount);
    void MoveRight  (float amount);
    void MoveUp     (float amount);

    // Rotate (yaw = around world-Y, pitch = around camera-X)
    void Rotate(float yawDelta, float pitchDelta);

    // Rebuild matrices from current position/orientation
    void Update();

    // ── Accessors ─────────────────────────────────────────────────────────────
    const glm::mat4& ViewMatrix()       const { return m_View; }
    const glm::mat4& ProjectionMatrix() const { return m_Projection; }
    glm::mat4        ViewProjection()   const { return m_Projection * m_View; }

    const glm::vec3& Position() const { return m_Position; }
    const glm::vec3& Forward()  const { return m_Forward; }

    void SetAspectRatio(float ratio);

private:
    void RebuildVectors();

    glm::vec3 m_Position = {0.0f, 0.0f,  3.0f};
    glm::vec3 m_Forward  = {0.0f, 0.0f, -1.0f};
    glm::vec3 m_Up       = {0.0f, 1.0f,  0.0f};
    glm::vec3 m_Right    = {1.0f, 0.0f,  0.0f};

    float m_Yaw         = -90.0f;  // face -Z initially
    float m_Pitch       =   0.0f;
    float m_PitchLimit  =  89.0f;  // prevent gimbal flip

    float m_Fov, m_Aspect, m_Near, m_Far;

    glm::mat4 m_View       = glm::mat4(1.0f);
    glm::mat4 m_Projection = glm::mat4(1.0f);
};

} // namespace Engine::Core
