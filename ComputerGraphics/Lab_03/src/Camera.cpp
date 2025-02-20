#include "Camera.h"

Camera::Camera()
    : position(0.0f, 0.0f, 5.0f),
      target(0.0f, 0.0f, 0.0f),
      up(0.0f, 1.0f, 0.0f),
      radius(5.0f),
      mouseSensitivity(0.05f),
      maxPitch(89.0f),
      minPitch(-89.0f),
      pitch(0.0f),
      yaw(0.0f) {}

void Camera::updateCameraPosition() {
    float radX = glm::radians(pitch);
    float radY = glm::radians(yaw);

    position.x = radius * cos(radX) * cos(radY);
    position.y = radius * sin(radX);
    position.z = radius * cos(radX) * sin(radY);
}

void Camera::mouseMove(float xOffset, float yOffset) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch -= yOffset;

    // Ограничение угла наклона
    if (pitch > maxPitch) pitch = maxPitch;
    if (pitch < minPitch) pitch = minPitch;

    updateCameraPosition();
}

void Camera::zoomIn(float deltaTime) {
    radius = std::max(1.0f, radius - deltaTime);
}

void Camera::zoomOut(float deltaTime) {
    radius = std::min(50.0f, radius + deltaTime);
}


glm::mat4 Camera::getProjectionViewMatrix() {
    glm::vec3 f = glm::normalize(target - position);
    glm::vec3 s = glm::normalize(glm::cross(f, up));
    glm::vec3 u = glm::cross(s, f);

    view = glm::mat4(1.0f);
    view[0][0] = s.x;
    view[1][0] = s.y;
    view[2][0] = s.z;
    view[0][1] = u.x;
    view[1][1] = u.y;
    view[2][1] = u.z;
    view[0][2] = -f.x;
    view[1][2] = -f.y;
    view[2][2] = -f.z;
    view[3][0] = -glm::dot(s, position);
    view[3][1] = -glm::dot(u, position);
    view[3][2] = glm::dot(f, position);

    return projection * view;
}

void Camera::setupProjection(int width, int height) {
    projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
}
