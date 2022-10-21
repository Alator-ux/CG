#pragma once
#include "GL/glew.h"
#include "Figure.h"
#include "PrimitiveChanger.h"
#include "Matrixes.h"


void shift(Figure* figure, glm::vec3 vec) {
    auto center = figure->center();
    auto d = vec - center;
    auto shift_matrix = build_shift_matrix(d);
    figure->transform(shift_matrix);
}


void scale(Figure* figure, glm::vec3 vec){
    auto scale_matrix = build_scale_matrix(vec);
    figure->transform(scale_matrix);
}

void scale_around_center(Figure* figure, glm::vec3 vec) {
    auto around = figure->center();
    auto shift1 = build_shift_matrix(around);
    auto scale = build_scale_matrix(vec);
    auto shift2 = build_shift_matrix(glm::vec3(0.0f));
    auto tricky_matrix = shift1 * scale * shift2;
    figure->transform(tricky_matrix);
}

void rotate(Figure* figure, Axis axis, float angle) {
    auto rotation_matrix = build_rotation_matrix(axis, angle);
    figure->transform(rotation_matrix);
}

void rotate_around_line(Figure* figure, float angle, glm::vec3 p1, glm::vec3 p2)
{
    //матрица // какая матрица? Ты шиз?
    if (p2.z < p1.z || (p2.z == p1.z && p2.y < p1.y) ||
        (p2.z == p1.z && p2.y == p1.y) && p2.x < p1.x)
    {
        swap(p1, p2);
    }

    auto center = figure->center();
    auto d = p1 - center;
    auto shift_matrix = build_shift_matrix(-p1);
    auto shift_matrix2 = build_shift_matrix(p1);

    glm::vec3 rotate_point = p2 - p1;//прямая, вокруг которой будем вращать
    double length = sqrt(rotate_point.x * rotate_point.x + rotate_point.y * rotate_point.y + rotate_point.z * rotate_point.z);
    double l = rotate_point.x / length;
    double m = rotate_point.y / length;
    double n = rotate_point.z / length;
    
    double angle_sin = sin(toRadians(angle));
    double angle_cos = cos(toRadians(angle));

    auto rotation_matrix = glm::mat4x4(
        l * l + angle_cos * (1 - l * l), l * (1 - angle_cos) * m - n * angle_sin, l * (1 - angle_cos) * n + m * angle_sin, 0,
        l * (1 - angle_cos) * m + n * angle_sin, m * m + angle_cos * (1 - m * m), m * (1 - angle_cos) * n - l * angle_sin, 0,
        l * (1 - angle_cos) * n - m * angle_sin, m * (1 - angle_cos) * n + l * angle_sin, n * n + angle_cos * (1 - n * n), 0,
        0, 0, 0, 1);

    auto tricky_matrix = shift_matrix2 * rotation_matrix * shift_matrix; // oh, that's a tricky one
    figure->transform(tricky_matrix);

}

void rotate_around_center(Figure* figure, Axis axis, float angle) {
    auto around = figure->center();
    auto shift1 = build_shift_matrix(around);
    auto rotate = build_rotation_matrix(axis, angle);
    auto shift2 = build_shift_matrix(glm::vec3(0.0f));
    auto tricky_matrix = shift1 * rotate * shift2;
    
    figure->transform(tricky_matrix);
}

// Отражение относительно выбранной координатной плоскости
void reflection_about_the_axis(Figure* figure, Axis axis)
{
    auto reflection_matrix = build_reflection_matrix(axis);
    figure->transform(reflection_matrix);
}

