#include "camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera *Camera::instance = nullptr;
std::mutex Camera::mtx;

glm::mat4 Camera::getViewMatrix() const { return lookAt(pos, target, up()); }

glm::mat4 Camera::getProjectionMatrix() const { return glm::perspective(glm::radians(fov), aspectRatio(), near, far); }
