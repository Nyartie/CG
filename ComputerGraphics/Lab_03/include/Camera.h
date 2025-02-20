#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();

    void updateCameraPosition();
    void mouseMove(float xOffset, float yOffset);
    glm::mat4 getProjectionViewMatrix();
    void setupProjection(int width, int height);
    void zoomIn(float deltaTime);
    void zoomOut(float deltaTime);

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float radius;
    float mouseSensitivity;
    float maxPitch;
    float minPitch;
    float pitch; 
    float yaw;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif
