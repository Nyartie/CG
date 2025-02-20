#include <iostream> 
#include <SFML/Window.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Pyramid.h" 


const char* vertexShaderSource = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec3 aNormal;

    uniform mat4 projectionView;

    out vec3 vertexColor;
    out vec3 fragPos;
    out vec3 vertexNormal;

    void main() {
        gl_Position = projectionView * vec4(aPos, 1.0);
        vertexColor = aColor;
        vertexNormal = aNormal;
        fragPos = aPos;
    }
)";

const char* fragmentShaderSource = R"(
    #version 460 core
    in vec3 vertexColor;
    in vec3 fragPos;
    in vec3 vertexNormal;

    out vec4 FragColor;

    uniform vec3 lightPos; 
    uniform vec3 lightDir; 
    uniform float cutoff; 
    uniform float outerCutoff; 
    uniform vec3 lightColor;
    uniform vec3 objectColor;

    void main() {
        vec3 fragToLight = normalize(lightPos - fragPos);
        
        float theta = dot(fragToLight, normalize(-lightDir));

        vec3 normal = normalize(vertexNormal);
        float diffuseStrength = max(dot(normal, fragToLight), 0.0);

        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;

        vec3 result = ambient * vertexColor;
        
        if(theta > outerCutoff) {
            float epsilon = cutoff - outerCutoff;
            float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

            vec3 diffuse = diffuseStrength * lightColor * intensity;

            float distance = length(lightPos - fragPos);
            float attenuation = 1.0 / (1.0 + 0.045 * distance + 0.0075 * distance * distance);
            
            result = (ambient + diffuse * attenuation) * vertexColor;
        }
        
        FragColor = vec4(result, 1.0);
    }
)";

void checkShaderCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
}

unsigned int createShaderProgram() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileErrors(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileErrors(fragmentShader, "FRAGMENT");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
int main() {
    sf::Window window(sf::VideoMode(800, 600), "3D Spotlight and Pyramid", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    Pyramid pyramid(1.0f, 1.5f);
    Camera camera;
    camera.setupProjection(800, 600);

    unsigned int shaderProgram = createShaderProgram();

    unsigned int projectionViewLocation = glGetUniformLocation(shaderProgram, "projectionView");
    unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
    unsigned int lightDirLocation = glGetUniformLocation(shaderProgram, "lightDir");
    unsigned int cutoffLocation = glGetUniformLocation(shaderProgram, "cutoff");
    unsigned int outerCutoffLocation = glGetUniformLocation(shaderProgram, "outerCutoff");
    unsigned int lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");
    unsigned int objectColorLocation = glGetUniformLocation(shaderProgram, "objectColor");

    // Параметры прожектора
    glm::vec3 lightPos(0.0f, 4.0f, 0.0f); 
    glm::vec3 lightDir = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)); 
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
    float cutoff = glm::cos(glm::radians(10.0f));    
    float outerCutoff = glm::cos(glm::radians(10.0f));   
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        float lightMoveSpeed = 2.0f * deltaTime;
        float lightRotateSpeed = 2.0f * deltaTime;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            camera.rotateLeft(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            camera.rotateRight(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            camera.moveUp(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            camera.moveDown(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            camera.zoomIn(deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            camera.zoomOut(deltaTime);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            lightPos.x -= lightMoveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            lightPos.x += lightMoveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            lightPos.z -= lightMoveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            lightPos.z += lightMoveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            lightPos.y += lightMoveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            lightPos.y -= lightMoveSpeed;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
            lightDir.x -= lightRotateSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            lightDir.x += lightRotateSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
            lightDir.y += lightRotateSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            lightDir.y -= lightRotateSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
            lightDir.z -= lightRotateSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            lightDir.z += lightRotateSpeed;
        

        lightDir = glm::normalize(lightDir);


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);


        glm::mat4 projectionView = camera.getProjectionViewMatrix();
        glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, glm::value_ptr(projectionView));
        glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPos));
        glUniform3fv(lightDirLocation, 1, glm::value_ptr(lightDir));
        glUniform1f(cutoffLocation, cutoff);
        glUniform1f(outerCutoffLocation, outerCutoff);
        glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
        glUniform3fv(objectColorLocation, 1, glm::value_ptr(objectColor));
        pyramid.draw(); 

        window.display();
    }

    glDeleteProgram(shaderProgram);
    return 0;
}
