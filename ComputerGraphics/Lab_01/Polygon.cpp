#include "Polygon.hpp"
#include <cmath>

const float PI = 3.14159265359f;

mPolygon::mPolygon(int sides, float radius) : sides(sides), radius(radius) {
    createPolygon();
}

void mPolygon::createPolygon() {
    float angleStep = 2 * PI / sides;
    for (int i = 0; i < sides; ++i) {
        float angle = i * angleStep - PI / 2;
        vertices.push_back(radius * cos(angle));  // x
        vertices.push_back(radius * sin(angle));  // y
        vertices.push_back(0.0f);                 // z
    }
}

std::vector<float> mPolygon::transform(const Matrix3x3& matrix) {
    std::vector<float> transformedVertices = vertices;
    for (size_t i = 0; i < transformedVertices.size(); i += 3) {
        float x = transformedVertices[i];
        float y = transformedVertices[i + 1];
        matrix.apply(x, y);
        transformedVertices[i] = x;
        transformedVertices[i + 1] = y;
    }
    return transformedVertices;
}