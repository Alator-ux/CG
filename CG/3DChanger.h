#pragma once
#include "GL/glew.h"
#include "Figure.h"
#include "PrimitiveChanger.h"

void shift(Figure* figure, glm::vec3 vec) {
    auto center = figure->center();
    auto d = vec - center;
    auto shiftMatrix = glm::mat4x4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        d.x, d.y, d.z, 1
    );
    figure->transform([shiftMatrix](glm::vec3 p)->glm::vec3 {
        auto res = shiftMatrix * glm::vec4(p.x, p.y, p.z, 1);
        return res;
        });
}


void scale(Figure* figure, glm::vec3 vec){
    auto scaleMatrix = glm::mat4x4(
        vec.x, 0, 0, 0,
        0, vec.y, 0, 0,
        0, 0, vec.z, 0,
        0, 0, 0, 1
    );
    figure->transform([scaleMatrix](glm::vec3 p)->glm::vec3 {
        auto res = scaleMatrix * glm::vec4(p.x, p.y, p.z, 1);
        return res;
        });
}

void scale_around_center(Figure* figure, glm::vec3 vec) {
    auto around = figure->center();
    shift(figure, glm::vec3(0.f));
    scale(figure, vec);
    shift(figure, around);
}

void rotate(Figure* figure, Axis axis, float angle) {
    float radianAngle = toRadians(angle);
    auto angleCos = cos(radianAngle);
    auto angleSin = sin(radianAngle);
    glm::mat4x4 rotationMatrix;
    switch (axis) {
    case Axis::ox:
        rotationMatrix = glm::mat4x4(
            1,0,0,0,
            0,angleCos,-angleSin,0,
            0,angleSin,angleCos,0,
            0,0,0,1
        );
        break;
    case Axis::oy:
        rotationMatrix = glm::mat4x4(
            angleCos, 0, angleSin, 0,
            0, 1, 0, 0,
            -angleSin,0, angleCos, 0,
            0, 0, 0, 1
        );
        break;
    case Axis::oz:
        rotationMatrix = glm::mat4x4(
            angleCos, -angleSin, 0, 0,
            angleSin, angleCos, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
        break;
    }
    figure->transform([rotationMatrix](glm::vec3 p)->glm::vec3 {
        auto res = rotationMatrix * glm::vec4(p.x, p.y, p.z, 1);
        return res;
        });
}

void rotate_around_line(Figure* figure, float angle, glm::vec3 p1, glm::vec3 p2)
{
    //матрица
    if (p2.z < p1.z || (p2.z == p1.z && p2.y < p1.y) ||
        (p2.z == p1.z && p2.y == p1.y) && p2.x < p1.x)
    {
        auto tmp = p1;
        p1 = p2;
        p2 = tmp;
    }

    auto center = figure->center();
    auto d = p1 - center;
    auto shiftMatrix = glm::mat4x4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -p1.x, -p1.y, -p1.z, 1
    );
    auto shiftMatrix2 = glm::mat4x4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        p1.x, p1.y, p1.z, 1
    );

    glm::vec3 rotate_point = p2 - p1;//прямая, вокруг которой будем вращать
    double length = sqrt(rotate_point.x * rotate_point.x + rotate_point.y * rotate_point.y + rotate_point.z * rotate_point.z);
    double l = rotate_point.x / length;
    double m = rotate_point.y / length;
    double n = rotate_point.z / length;
    
    double anglesin = sin(toRadians(angle));
    double anglecos = cos(toRadians(angle));
    auto rotation_matrix = glm::mat4x4(
        l * l + anglecos * (1 - l * l), l * (1 - anglecos) * m - n * anglesin, l * (1 - anglecos) * n + m * anglesin, 0,
        l * (1 - anglecos) * m + n * anglesin, m * m + anglecos * (1 - m * m), m * (1 - anglecos) * n - l * anglesin, 0,
        l * (1 - anglecos) * n - m * anglesin, m * (1 - anglecos) * n + l * anglesin, n * n + anglecos * (1 - n * n), 0,
        0, 0, 0, 1);
    auto res_matr = shiftMatrix2 * rotation_matrix * shiftMatrix;
    figure->transform([res_matr](glm::vec3 p)->glm::vec3 {
        auto res = res_matr * glm::vec4(p.x, p.y, p.z, 1);
        return res;
        });

}

void rotate_around_center(Figure* figure, Axis axis, float angle) {
    auto center = figure->center();
    shift(figure, glm::vec3(0.f));
    rotate(figure, axis, angle);
    shift(figure, center);
}

// Отражение относительно выбранной координатной плоскости
void reflectionAboutTheAxis(Figure* figure, Axis axis)
{
    glm::mat4x4 reflectionMatrix;
    switch (axis)
    {
    case Axis::ox:                
        reflectionMatrix = glm::mat4x4(
            1, 0, 0, 0,
            0, -1, 0, 0,
            0, 0, -1, 0, 
            0, 0, 0, 1);
        break;
    case Axis::oy: 
        reflectionMatrix = glm::mat4x4(
            -1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, -1, 0, 
            0,0, 0, 1);
        break;
    case Axis::oz:
        reflectionMatrix = glm::mat4x4(
            -1, 0, 0, 0,
            0, -1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        break;
    }

    // отражение фигуры
    figure->transform([reflectionMatrix](glm::vec3 p)->glm::vec3 {
        auto res = reflectionMatrix * glm::vec4(p.x, p.y, p.z, 1);
        return res;
        });
}

