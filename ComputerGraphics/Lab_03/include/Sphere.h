#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <vector>
#include <glm/glm.hpp>

class Sphere {
public:
    Sphere(int numSlices, float radius);
    void draw() const;

private:
    void initialize(int numSlices, float radius);
    void setupBuffers();
    unsigned int VAO, VBO, EBO;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
    std::vector<unsigned int> indices;
};

#endif
