#include "Camera.h"

Camera::Camera()
    : position(0.0f, 0.0f, 5.0f), 
      target(0.0f, 0.0f, 0.0f), 
      up(0.0f, 1.0f, 0.0f), 
      radius(5.0f), 
      height(0.0f), 
      angleX(0.0f), 
      angleY(0.0f) {}

void Camera::setupProjection(int width, int height) {
    float aspectRatio = static_cast<float>(width) / height;
    float fov = 45.0f * M_PI / 180.0f;
    float zNear = 0.1f;
    float zFar = 50.0f;


    float f = 1.0f / std::tan(fov / 2.0f);
    projection = glm::mat4(0.0f);
    projection[0][0] = f / aspectRatio;
    projection[1][1] = f;
    projection[2][2] = (zFar + zNear) / (zNear - zFar);
    projection[2][3] = -1.0f;
    projection[3][2] = (2.0f * zFar * zNear) / (zNear - zFar);
}

void Camera::rotateLeft(float deltaTime) {
    angleX -= 50.0f * deltaTime;
}

void Camera::rotateRight(float deltaTime) {
    angleX += 50.0f * deltaTime;
}

void Camera::moveUp(float deltaTime) {
    height += 2.0f * deltaTime;
}

void Camera::moveDown(float deltaTime) {
    height -= 2.0f * deltaTime;
}

void Camera::zoomIn(float deltaTime) {
    radius = std::max(1.0f, radius - deltaTime);
}

void Camera::zoomOut(float deltaTime) {
    radius = std::min(50.0f, radius + deltaTime);
}

glm::mat4 Camera::getProjectionViewMatrix() {
    // Ручной расчет позиции камеры
    float radX = angleX * M_PI / 180.0f;
    float radY = angleY * M_PI / 180.0f;
    
    position.x = radius * std::cos(radX) * std::cos(radY);
    position.y = height;
    position.z = radius * std::sin(radX) * std::cos(radY);

    // Ручной расчет матрицы вида
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