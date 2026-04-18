#include "core/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace Engine::Core {

Camera::Camera(float fovDegrees, float aspectRatio, float nearZ, float farZ)
    : m_Fov(fovDegrees), m_Aspect(aspectRatio), m_Near(nearZ), m_Far(farZ)
{
    m_Projection = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);
    RebuildVectors();
    Update();
}

void Camera::MoveForward(float amount) { m_Position += m_Forward * amount; }
void Camera::MoveRight  (float amount) { m_Position += m_Right   * amount; }
void Camera::MoveUp     (float amount) { m_Position += glm::vec3(0,1,0)  * amount; }

void Camera::Rotate(float yawDelta, float pitchDelta) {
    m_Yaw   += yawDelta;
    m_Pitch  = std::clamp(m_Pitch + pitchDelta, -m_PitchLimit, m_PitchLimit);
    RebuildVectors();
}

void Camera::Update() {
    m_View = glm::lookAt(m_Position, m_Position + m_Forward, glm::vec3(0,1,0));
}

void Camera::SetAspectRatio(float ratio) {
    m_Aspect     = ratio;
    m_Projection = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);
}

void Camera::RebuildVectors() {
    float yawR   = glm::radians(m_Yaw);
    float pitchR = glm::radians(m_Pitch);

    m_Forward = glm::normalize(glm::vec3{
        cos(pitchR) * cos(yawR),
        sin(pitchR),
        cos(pitchR) * sin(yawR)
    });
    m_Right = glm::normalize(glm::cross(m_Forward, glm::vec3(0,1,0)));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Forward));
}

} // namespace Engine::Core
