#ifndef CAMERA_HPP
#define CAMERA_HPP
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>
#include <SFML/Window.hpp>

class Camera {
public:
    Camera();
    void setupProjection(int width, int height);
    void rotateLeft(float deltaTime);
    void rotateRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);
    void zoomIn(float deltaTime);
    void zoomOut(float deltaTime);

    glm::mat4 getProjectionViewMatrix();

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float radius;
    float height;
    float angleX;
    float angleY;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif
