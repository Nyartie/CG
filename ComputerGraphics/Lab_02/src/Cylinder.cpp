#include "Cylinder.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <cmath>
#include <iostream>

Cylinder::Cylinder(int numSlices, float radius, float height) {
    initialize(numSlices, radius, height);
}

void Cylinder::initialize(int numSlices, float radius, float height) {
    vertices.clear();
    colors.clear();
    indices.clear();

    numSlices = std::max(3, numSlices); 

    float angleStep = 360.0f / numSlices;

    glm::vec3 baseColor(1.0f, 0.0f, 0.0f); 

    for (int i = 0; i < numSlices; ++i) {
        float angle = glm::radians(i * angleStep);
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        float shade = static_cast<float>(i) / numSlices;
        glm::vec3 currentColor = glm::vec3(baseColor.r * (1.0f - shade), baseColor.g * (1.0f - shade), baseColor.b);

        vertices.push_back(glm::vec3(x, -height / 2.0f, z));
        colors.push_back(currentColor);

        vertices.push_back(glm::vec3(x, height / 2.0f, z));
        colors.push_back(currentColor);
    }

    vertices.push_back(glm::vec3(0.0f, -height / 2.0f, 0.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

    vertices.push_back(glm::vec3(0.0f, height / 2.0f, 0.0f));
    colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));

    for (int i = 0; i < numSlices; ++i) {
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 1);
        indices.push_back((i + 1) % numSlices * 2);

        indices.push_back(i * 2 + 1);
        indices.push_back((i + 1) % numSlices * 2 + 1);
        indices.push_back((i + 1) % numSlices * 2);
    }

    int baseCenterIndex = numSlices * 2;
    for (int i = 0; i < numSlices; ++i) {
        indices.push_back(baseCenterIndex);
        indices.push_back(i * 2);
        indices.push_back(((i + 1) % numSlices) * 2);
    }

    int topCenterIndex = numSlices * 2 + 1;
    for (int i = 0; i < numSlices; ++i) {
        indices.push_back(topCenterIndex);
        indices.push_back(((i + 1) % numSlices) * 2 + 1);
        indices.push_back(i * 2 + 1);
    }
    
    setupBuffers();
}

void Cylinder::setupBuffers(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    size_t vertexSize = vertices.size() * sizeof(glm::vec3);
    size_t colorSize = colors.size() * sizeof(glm::vec3);

    glBufferData(GL_ARRAY_BUFFER, vertexSize + colorSize, nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertices.data());

    glBufferSubData(GL_ARRAY_BUFFER, vertexSize, colorSize, colors.data());


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)vertexSize);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


void Cylinder::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
