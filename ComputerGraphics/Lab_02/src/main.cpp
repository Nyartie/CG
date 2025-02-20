#include <iostream> 
#include <SFML/Window.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cylinder.h"
#include "Camera.h"

const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    
    uniform mat4 projectionView;
    
    out vec3 vertexColor;
    
    void main() {
        gl_Position = projectionView * vec4(aPos, 1.0);
        vertexColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 460 core
    in vec3 vertexColor;
    out vec4 FragColor;
    
    void main() {
        FragColor = vec4(vertexColor, 1.0);
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
    sf::Window window(sf::VideoMode(800, 600), "3D Cylinder", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Cylinder cylinder(30, 1.0f, 2.0f);
    Camera camera;
    camera.setupProjection(800, 600);

    unsigned int shaderProgram = createShaderProgram();
    unsigned int projectionViewLocation = glGetUniformLocation(shaderProgram, "projectionView");

    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            camera.rotateLeft(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            camera.rotateRight(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            camera.moveUp(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            camera.moveDown(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            camera.zoomIn(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            camera.zoomOut(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 projectionView = camera.getProjectionViewMatrix();
        glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, glm::value_ptr(projectionView));

        cylinder.draw();

        window.display();
    }

    glDeleteProgram(shaderProgram);
    return 0;
}