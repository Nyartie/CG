#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include "Matrix.hpp"
#include "Polygon.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Polygon Transformation");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    mPolygon polygon(10, 0.3f);
    Matrix3x3 transformMatrix;

    sf::ConvexShape convexShape;
    convexShape.setPointCount(polygon.sides);

    float moveX = 0.0f, moveY = 0.0f;
    float scale = 1.0f;
    float rotation = 0.0f;

    bool circularMotion = false;
    float circleRadius = 0.3f;
    float circleAngle = 0.0f;
    float circleSpeed = 3.0f;

    float moveSpeed = 2.0f;
    float scaleSpeed = 2.0f;
    float rotationSpeed = 200.0f;

    sf::Clock clock;
    while (window.isOpen()) {
        float delta_time = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q) {
                circularMotion = !circularMotion;
                circleAngle = 0.0f;
            }
        }
        if (!circularMotion) {
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                moveY += moveSpeed * delta_time;
                // Ограничение по верхней границе
                moveY = std::min(moveY, 1.0f - scale * 0.2f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                moveY -= moveSpeed * delta_time;
                // Ограничение по нижней границе
                moveY = std::max(moveY, -1.0f + scale * 0.2f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                moveX -= moveSpeed * delta_time;
                // Ограничение по левой границе
                moveX = std::max(moveX, -1.0f + scale * 0.2f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                moveX += moveSpeed * delta_time;
                // Ограничение по правой границе
                moveX = std::min(moveX, 1.0f - scale * 0.2f);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                scale += scaleSpeed * delta_time;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                scale -= scaleSpeed * delta_time;
                scale = std::max(0.1f, scale);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) rotation -= rotationSpeed * delta_time;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) rotation += rotationSpeed * delta_time;
        }
        else {
            circleAngle += circleSpeed * delta_time;
            // Уравнения для движения по восьмёрке
            moveX = (0.5f * sin(circleAngle));
            moveY = (0.5f * sin(2 * circleAngle));
            rotation += rotationSpeed * delta_time;
        }

        transformMatrix.setIdentity();
        transformMatrix = transformMatrix
            .translate(moveX, moveY)
            .scale(scale, scale)
            .rotate(rotation);

        std::vector<float> transformedVertices = polygon.transform(transformMatrix);

        for (int i = 0; i < polygon.sides; ++i) {
            convexShape.setPoint(i, sf::Vector2f(
                transformedVertices[i*3] * 400 + 400, 
                -transformedVertices[i*3 + 1] * 300 + 300
            ));
        }

        window.clear();
        window.draw(convexShape);
        window.display();
    }

    return 0;
}