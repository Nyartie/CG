#include "Matrix.hpp"

const float PI = 3.14159265359f;

Matrix3x3::Matrix3x3() {
    for (int i = 0; i < 9; ++i) {
        m[i] = 0.0f;
    }
}

void Matrix3x3::setIdentity() {
    m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f;
    m[3] = 0.0f; m[4] = 1.0f; m[5] = 0.0f;
    m[6] = 0.0f; m[7] = 0.0f; m[8] = 1.0f;
}

Matrix3x3 Matrix3x3::multiply(const Matrix3x3& other) const {
    Matrix3x3 result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.m[i * 3 + j] = 0;
            for (int k = 0; k < 3; ++k) {
                result.m[i * 3 + j] += m[i * 3 + k] * other.m[k * 3 + j];
            }
        }
    }
    return result;
}

void Matrix3x3::apply(float& x, float& y) const {
    float newX = m[0] * x + m[1] * y + m[2];
    float newY = m[3] * x + m[4] * y + m[5];
    x = newX;
    y = newY;
}

Matrix3x3 Matrix3x3::createTranslation(float tx, float ty) {
    Matrix3x3 matrix;
    matrix.setIdentity();
    matrix.m[2] = tx;
    matrix.m[5] = ty;
    return matrix;
}

Matrix3x3 Matrix3x3::createScale(float sx, float sy) {
    Matrix3x3 matrix;
    matrix.setIdentity();
    matrix.m[0] = sx;
    matrix.m[4] = sy;
    return matrix;
}

Matrix3x3 Matrix3x3::createRotation(float angle) {
    Matrix3x3 matrix;
    matrix.setIdentity();
    float rad = angle * PI / 180.0f;
    matrix.m[0] = cos(rad);
    matrix.m[1] = -sin(rad);
    matrix.m[3] = sin(rad);
    matrix.m[4] = cos(rad);
    return matrix;
}

Matrix3x3 Matrix3x3::translate(float tx, float ty) const {
    Matrix3x3 translationMatrix = createTranslation(tx, ty);
    return multiply(translationMatrix);
}

Matrix3x3 Matrix3x3::scale(float sx, float sy) const {
    Matrix3x3 scaleMatrix = createScale(sx, sy);
    return multiply(scaleMatrix);
}

Matrix3x3 Matrix3x3::rotate(float angle) const {
    Matrix3x3 rotationMatrix = createRotation(angle);
    return multiply(rotationMatrix);
}