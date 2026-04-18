#include "RenderComponents.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Game {

glm::mat4 TransformComponent::Matrix() const {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), Position);

    t = glm::rotate(t, glm::radians(Rotation.x), {1, 0, 0});
    t = glm::rotate(t, glm::radians(Rotation.y), {0, 1, 0});
    t = glm::rotate(t, glm::radians(Rotation.z), {0, 0, 1});

    t = glm::scale(t, Scale);
    return t;
}

} // namespace Game
