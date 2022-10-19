#pragma once
#include "GL/glew.h"
#include "Figure.h"
#include "PrimitiveChanger.h"

void shift(Figure* figure, float xs, float ys, float zs) {
    auto shiftMatrix = glm::mat4x4(
        1, 0, 0, xs,
        0, 1, 0, ys,
        0, 0, 1, zs,
        0, 0, 0, 1
    );
    figure->transform([shiftMatrix](glm::vec3 p)->glm::vec3 {
        auto res = shiftMatrix * glm::vec4(p.x, p.y, p.z, 1); // TODO: ебанёт
        return glm::vec3(res[0, 0], res[1, 0], res[2, 0]);
        });
}



void scale(Figure* figure, float xs, float ys, float zs){
    auto scaleMatrix = glm::mat4x4(
        xs, 0, 0, 0,
        0, ys, 0, 0,
        0, 0, zs, 0,
        0, 0, 0, 1
    );
    figure->transform([scaleMatrix](glm::vec3 p)->glm::vec3 {
        auto res = scaleMatrix * glm::vec4(p.x, p.y, p.z, 1); // TODO: ебанёт
        return glm::vec3(res[0, 0], res[1, 0], res[2, 0]);
        });
}


void scaleNshift(Figure* figure, float scalex, float scaley, float scalez, float shiftx, float shifty, float shiftz) {
    auto magicMatrix = glm::mat4x4(
        scalex, 0, 0, 0,
        0, scaley, 0, 0,
        0, 0, scalez, 0,
        (1-scalex)*shiftx, (1 - scaley) * shifty, (1 - scalez) * shiftz, 1
    );
    figure->transform([magicMatrix](glm::vec3 p)->glm::vec3 {
        auto res = magicMatrix * glm::vec4(p.x, p.y, p.z, 1); // TODO: ебанёт
        return glm::vec3(res[0, 0], res[1, 0], res[2, 0]);
        });
}

void rotate(Figure* figure, Axis axis, float angle) {
    float radianAngle = toRadians(angle);
    auto angleCos = cos(radianAngle);
    auto angleSin = cos(radianAngle);
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
        auto res = rotationMatrix * glm::vec4(p.x, p.y, p.z, 1); // TODO: ебанёт
        return glm::vec3(res[0, 0], res[1, 0], res[2, 0]);
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

    glm::vec3 rotate_point = glm::vec3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);//прямая, вокруг которой будем вращать

    double length = sqrt(rotate_point.x * rotate_point.x + rotate_point.y * rotate_point.y + rotate_point.z * rotate_point.z);
    double l = rotate_point.x / length;
    double m = rotate_point.x / length;
    double n = rotate_point.z / length;
    
    double anglesin = sin(toRadians(angle));
    double anglecos = cos(toRadians(angle));
    auto rotation_matrix = glm::mat4x4(
        l * l + anglecos * (1 - l * l), l * (1 - anglecos) * m - n * anglesin, l * (1 - anglecos) * n + m * anglesin, 0,
        l * (1 - anglecos) * m + n * anglesin, m * m + anglecos * (1 - m * m), m * (1 - anglecos) * n - l * anglesin, 0,
        l * (1 - anglecos) * n - m * anglesin, m * (1 - anglecos) * n + l * anglesin, n * n + anglecos * (1 - n * n), 0,
        0, 0, 0, 1);

    figure->transform([rotation_matrix](glm::vec3 p)->glm::vec3 {
        auto res = rotation_matrix * glm::vec4(p.x, p.y, p.z, 1); // TODO: ебанёт
        return glm::vec3(res[0, 0], res[1, 0], res[2, 0]);
        });
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
            0, 1, 0, 0,
            0, 0, -1, 0, 
            0, 0, 0, 1);
        break;
    case Axis::oy: 
        reflectionMatrix = glm::mat4x4(
            1, 0, 0, 0,
            0, -1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1);
        break;
    case Axis::oz:
        reflectionMatrix = glm::mat4x4(
            -1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        break;
    }

    // отражение фигуры
    figure->transform([reflectionMatrix](glm::vec3 p)->glm::vec3 {
        auto res = reflectionMatrix * glm::vec4(p.x, p.y, p.z, 1); // TODO: ебанёт
        return glm::vec3(res[0, 0], res[1, 0], res[2, 0]);
        });
}

void rotationThroughTheCenter(Figure* figure, Axis axis, float angle)
{
    double sumX = 0, sumY = 0, sumZ = 0;
    for(auto face : figure->faces)
    {
        auto center = face.center();
        sumX += center.x;
        sumY += center.y;
        sumZ += center.z;
    }
    auto size = figure->faces.size();
    // центр фигуры
    glm::vec3 center = glm::vec3(sumX / size, sumY / size, sumZ / size);

    auto rotationMatrix = glm::mat4x4(1, 0, 0, -center.x, 0, 1, 0, -center.y, 0, 0, 1, -center.z, 0, 0, 0, 1);
    figure->transform([rotationMatrix](glm::vec3 p)->glm::vec3 {
        auto res = rotationMatrix * glm::vec4(p.x, p.y, p.z, 1); // TODO: ебанёт
        return glm::vec3(res[0, 0], res[1, 0], res[2, 0]);
    });

    // поворачиваем относительно оси
    rotate(figure, axis, angle);

    // возвращаем на исходное место
    rotationMatrix = glm::mat4x4(1, 0, 0, center.x, 0, 1, 0, center.y, 0, 0, 1, center.z, 0, 0, 0, 1);
    figure->transform([rotationMatrix](glm::vec3 p)->glm::vec3 {
        auto res = rotationMatrix * glm::vec4(p.x, p.y, p.z, 1); // TODO: ебанёт
        return glm::vec3(res[0, 0], res[1, 0], res[2, 0]);
    });

}

