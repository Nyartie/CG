#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightAngle; // Максимальный угол прожектора в радианах
uniform float exponent;   // Параметр затухания

void main()
{
    // Направление от текущей позиции пикселя к источнику света
    vec3 lightDir = normalize(lightPos - FragPos);

    // Вычисление угла между направлением света и нормалью
    float theta = dot(lightDir, normalize(Normal));

    // Если угол в пределах прожектора, применяем затухание
    if (theta > cos(lightAngle)) {
        float intensity = pow(theta, exponent);  // Затухание света
        FragColor = vec4(intensity * lightColor, 1.0);
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); // Если вне зоны прожектора
    }
}