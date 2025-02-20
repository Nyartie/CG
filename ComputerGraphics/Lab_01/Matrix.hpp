#pragma once
#include <cmath>

class Matrix3x3 {
public:
    float m[9];  

    Matrix3x3();
    void setIdentity();
    Matrix3x3 multiply(const Matrix3x3& other) const;
    void apply(float& x, float& y) const;

    static Matrix3x3 createTranslation(float tx, float ty);
    static Matrix3x3 createScale(float sx, float sy);
    static Matrix3x3 createRotation(float angle);

    Matrix3x3 translate(float tx, float ty) const;
    Matrix3x3 scale(float sx, float sy) const;
    Matrix3x3 rotate(float angle) const;
};