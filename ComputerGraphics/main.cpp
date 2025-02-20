#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class GameObject {
public:
    glm::vec3 position;
    glm::vec3 color;

    GameObject(glm::vec3 pos, glm::vec3 col) : position(pos), color(col) {}

    void draw() {
        glBegin(GL_QUADS);
        glColor3f(color.r, color.g, color.b);
        glVertex3f(position.x - 0.5f, position.y - 0.5f, position.z);
        glVertex3f(position.x + 0.5f, position.y - 0.5f, position.z);
        glVertex3f(position.x + 0.5f, position.y + 0.5f, position.z);
        glVertex3f(position.x - 0.5f, position.y + 0.5f, position.z);
        glEnd();
    }
};

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    Camera(glm::vec3 pos, glm::vec3 fwd, glm::vec3 upVec)
        : position(pos), front(fwd), up(upVec) {}

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }
};

int main() {
    sf::Window window(sf::VideoMode(800, 600), "Simple Game Engine", sf::Style::Close | sf::Style::Titlebar);
    window.setVerticalSyncEnabled(true);

    // Инициализация OpenGL
    glEnable(GL_DEPTH_TEST);

    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    GameObject cube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Красный куб

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Установка матрицы вида
        glm::mat4 view = camera.getViewMatrix();
        glLoadMatrixf(glm::value_ptr(view));

        // Рендеринг куба
        cube.draw();

        // Обновление окна
        window.display();
    }

    return 0;
}
