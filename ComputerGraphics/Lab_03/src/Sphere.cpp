#include "Sphere.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <cmath>
#include <iostream>

Sphere::Sphere(int numSlices, float radius) {
    initialize(numSlices, radius);
}

void Sphere::initialize(int numSlices, float radius) {
    vertices.clear();
    colors.clear();
    indices.clear();

    for (int i = 0; i <= numSlices; ++i) {
        float theta = i * M_PI / numSlices; 
        for (int j = 0; j <= numSlices; ++j) {
            float phi = j * 2 * M_PI / numSlices; 

            float x = radius * sin(theta) * cos(phi);
            float y = radius * cos(theta);
            float z = radius * sin(theta) * sin(phi);

            vertices.push_back(glm::vec3(x, y, z));

            float r = (float)i / numSlices; 
            float g = (float)j / numSlices; 
            float b = 0.5f;
            colors.push_back(glm::vec3(r, g, b));
        }
    }

    for (int i = 0; i < numSlices; ++i) {
        for (int j = 0; j < numSlices; ++j) {
            indices.push_back(i * (numSlices + 1) + j);
            indices.push_back((i + 1) * (numSlices + 1) + j);
            indices.push_back(i * (numSlices + 1) + (j + 1));

            indices.push_back((i + 1) * (numSlices + 1) + j);
            indices.push_back((i + 1) * (numSlices + 1) + (j + 1));
            indices.push_back(i * (numSlices + 1) + (j + 1));
        }
    }

    setupBuffers();
}


void Sphere::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    unsigned int colorVBO; 

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}


void Sphere::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
