#include <iostream>
#include <SFML/Window.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sphere.h"
#include "Camera.h"

const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 color; // Добавляем атрибут цвета
    out vec3 fragColor; // Передаем цвет во фрагментный шейдер
    uniform mat4 projectionView;
    void main() {
        gl_Position = projectionView * vec4(position, 1.0);
        fragColor = color; // Передаем цвет
    }
)";
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 color;
    in vec3 fragColor; // Получаем цвет из вершинного шейдера
    void main() {
        color = vec4(fragColor, 1.0); // Используем переданный цвет
    }
)";

unsigned int createShaderProgram() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    sf::Window window(sf::VideoMode(800, 600), "3D Sphere", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Sphere sphere(30, 1.0f);
    Camera camera;
    camera.setupProjection(800, 600);

    unsigned int shaderProgram = createShaderProgram();
    unsigned int projectionViewLocation = glGetUniformLocation(shaderProgram, "projectionView");

    sf::Clock clock;
    float lastMouseX = 400, lastMouseY = 300;
    sf::Mouse::setPosition(sf::Vector2i(400, 300));

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved) {
                float xOffset = event.mouseMove.x - lastMouseX;
                float yOffset = lastMouseY - event.mouseMove.y; 

                camera.mouseMove(xOffset, yOffset);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            camera.zoomIn(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            camera.zoomOut(deltaTime);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glm::mat4 projectionView = camera.getProjectionViewMatrix();
        glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, glm::value_ptr(projectionView));

        sphere.draw();

        window.display();
    }

    glDeleteProgram(shaderProgram);
    return 0;
}
