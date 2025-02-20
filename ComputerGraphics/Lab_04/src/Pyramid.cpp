#include "Pyramid.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

Pyramid::Pyramid(float baseSize, float height) {
    initialize(baseSize, height);
}

void Pyramid::initialize(float baseSize, float height) {
    vertices.clear();
    colors.clear();
    indices.clear();

    vertices = {
        glm::vec3(0.0f, height, 0.0f),
        glm::vec3(-baseSize, 0.0f, -baseSize),
        glm::vec3(baseSize, 0.0f, -baseSize),

        glm::vec3(0.0f, height, 0.0f),
        glm::vec3(baseSize, 0.0f, -baseSize),
        glm::vec3(baseSize, 0.0f, baseSize),

        glm::vec3(0.0f, height, 0.0f),
        glm::vec3(baseSize, 0.0f, baseSize),
        glm::vec3(-baseSize, 0.0f, baseSize),

        glm::vec3(0.0f, height, 0.0f),
        glm::vec3(-baseSize, 0.0f, baseSize),
        glm::vec3(-baseSize, 0.0f, -baseSize),

        glm::vec3(-baseSize, 0.0f, -baseSize),
        glm::vec3(baseSize, 0.0f, -baseSize),
        glm::vec3(baseSize, 0.0f, baseSize),
        glm::vec3(-baseSize, 0.0f, baseSize)
    };

    normals = {
        glm::vec3(0.0f, 0.5f, -1.0f),
        glm::vec3(0.0f, 0.5f, -1.0f),
        glm::vec3(0.0f, 0.5f, -1.0f),

        glm::vec3(1.0f, 0.5f, 0.0f),
        glm::vec3(1.0f, 0.5f, 0.0f),
        glm::vec3(1.0f, 0.5f, 0.0f),

        glm::vec3(0.0f, 0.5f, 1.0f),
        glm::vec3(0.0f, 0.5f, 1.0f),
        glm::vec3(0.0f, 0.5f, 1.0f),

        glm::vec3(-1.0f, 0.5f, 0.0f),
        glm::vec3(-1.0f, 0.5f, 0.0f),
        glm::vec3(-1.0f, 0.5f, 0.0f),

        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
    };

    colors = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),

        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),

        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),

        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),

        glm::vec3(1.0f, 0.5f, 0.0f),
        glm::vec3(1.0f, 0.5f, 0.0f),
        glm::vec3(1.0f, 0.5f, 0.0f),
        glm::vec3(1.0f, 0.5f, 0.0f)
    };


    indices = {
        0, 1, 2,   
        3, 4, 5,   
        6, 7, 8,   
        9, 10, 11, 
        12, 13, 14,
        14, 15, 12 
    };

    setupBuffers();
}

void Pyramid::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);  

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    size_t vertexSize = vertices.size() * sizeof(glm::vec3);
    size_t colorSize = colors.size() * sizeof(glm::vec3);

    glBufferData(GL_ARRAY_BUFFER, vertexSize + colorSize, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize, vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertexSize, colorSize, colors.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)vertexSize);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Pyramid::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}