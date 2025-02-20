#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include <vector>
#include <glm/glm.hpp>

class Cylinder {
public:
    Cylinder(int numSlices, float radius, float height);
    void draw() const;

private:
    void initialize(int numSlices, float radius, float height);
    void setupBuffers();
    unsigned int VAO, VBO, EBO;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<unsigned int> indices;
};

#endif