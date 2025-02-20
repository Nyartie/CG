#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <vector>
#include <glm/glm.hpp>

class Pyramid {
public:
    Pyramid(float baseSize, float height);
    void draw() const;

private:
    void initialize(float baseSize, float height);
    void setupBuffers();
    unsigned int VAO, VBO, EBO, NBO;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
};

#endif
