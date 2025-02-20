#pragma once
#include <vector>
#include <SFML/OpenGL.hpp>
#include "Matrix.hpp"

class mPolygon {
public:
    std::vector<float> vertices;
    int sides;
    float radius;

    mPolygon(int sides, float radius);
    void createPolygon();
    std::vector<float> transform(const Matrix3x3& matrix);
};