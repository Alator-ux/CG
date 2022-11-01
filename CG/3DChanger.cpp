#include "3DChanger.h"
#include "RotationFigure.h"
#include "Figure.h"
#include "Matrixes.h"
#include "FunctionFigure.h"

void rotate_line1(primitives::Line* obj, Axis axis, float angle) {
    auto rotation_matrix = build_rotation_matrix(axis, angle);
    obj->transform(rotation_matrix);
}

void rotate(ThreeDInterface* obj, Axis axis, float angle) {
    auto rotation_matrix = build_rotation_matrix(axis, angle);
    switch (obj->type)
    {
    case ThreeDTypes::figure:
    {
        Figure* figure = reinterpret_cast<Figure*>(obj);
        figure->transform(rotation_matrix);
        break;
    }
    case ThreeDTypes::rotation_figure:
    {
        RotationFigure* figure = reinterpret_cast<RotationFigure*>(obj);
        figure->transform(rotation_matrix);
        break;
    }
    case ThreeDTypes::function_figure:
    {
        FunctionFigure* figure = reinterpret_cast<FunctionFigure*>(obj);
        figure->transform(rotation_matrix);
        break;
    }
    case ThreeDTypes::line:
    {
        primitives::Polygon* figure = reinterpret_cast<primitives::Polygon*>(obj);
        auto a = 1;
        //figure->transform(rotation_matrix);
        break;
    }
    default:
        break;
    }
}



void shift(ThreeDInterface* obj, glm::vec3 vec) {
    switch (obj->type)
    {
    case ThreeDTypes::figure:
    {
        Figure* figure = reinterpret_cast<Figure*>(obj);
        auto center = figure->center();
        auto d = vec - center;
        auto shift_matrix = build_shift_matrix(d);
        figure->transform(shift_matrix);
        break;
    }
    case ThreeDTypes::rotation_figure:
    {
        RotationFigure* figure = reinterpret_cast<RotationFigure*>(obj);
        auto center = figure->center();
        auto d = vec - center;
        auto shift_matrix = build_shift_matrix(d);
        figure->transform(shift_matrix);
        break;
    }
    case ThreeDTypes::function_figure:
    {
        FunctionFigure* figure = reinterpret_cast<FunctionFigure*>(obj);
        auto center = figure->center();
        auto d = vec - center;
        auto shift_matrix = build_shift_matrix(d);
        figure->transform(shift_matrix);
        break;
    }
    case ThreeDTypes::line:
    {
        primitives::Line* figure = reinterpret_cast<primitives::Line*>(obj);
        auto center = figure->center();
        auto d = vec - center;
        auto shift_matrix = build_shift_matrix(d);
        figure->transform(shift_matrix);
        break;
    }
    default:
        break;
    }
}

void scale(ThreeDInterface* obj, glm::vec3 vec) {
    auto scale_matrix = build_scale_matrix(vec);
    switch (obj->type)
    {
    case ThreeDTypes::figure:
    {
        Figure* figure = reinterpret_cast<Figure*>(obj);
        figure->transform(scale_matrix);
        break;
    }
    case ThreeDTypes::rotation_figure:
    {
        RotationFigure* figure = reinterpret_cast<RotationFigure*>(obj);
        figure->transform(scale_matrix);
        break;
    }
    case ThreeDTypes::function_figure:
    {
        FunctionFigure* figure = reinterpret_cast<FunctionFigure*>(obj);
        figure->transform(scale_matrix);
        break;
    }
    case ThreeDTypes::line:
    {
        primitives::Line* figure = reinterpret_cast<primitives::Line*>(obj);
        figure->transform(scale_matrix);
        break;
    }
    default:
        break;
    }
}

void scale_around_center(ThreeDInterface* obj, glm::vec3 vec) {
    switch (obj->type)
    {
    case ThreeDTypes::figure:
    {
        Figure* figure = reinterpret_cast<Figure*>(obj);
        auto around = figure->center();
        auto shift1 = build_shift_matrix(around);
        auto scale = build_scale_matrix(vec);
        auto shift2 = build_shift_matrix(glm::vec3(0.0f));
        auto tricky_matrix = shift1 * scale * shift2;
        figure->transform(tricky_matrix);
        break;
    }
    case ThreeDTypes::rotation_figure:
    {
        RotationFigure* figure = reinterpret_cast<RotationFigure*>(obj);
        auto around = figure->center();
        auto shift1 = build_shift_matrix(around);
        auto scale = build_scale_matrix(vec);
        auto shift2 = build_shift_matrix(glm::vec3(0.0f));
        auto tricky_matrix = shift1 * scale * shift2;
        figure->transform(tricky_matrix);
        break;
    }
    case ThreeDTypes::function_figure:
    {
        FunctionFigure* figure = reinterpret_cast<FunctionFigure*>(obj);
        auto around = figure->center();
        auto shift1 = build_shift_matrix(around);
        auto scale = build_scale_matrix(vec);
        auto shift2 = build_shift_matrix(glm::vec3(0.0f));
        auto tricky_matrix = shift1 * scale * shift2;
        figure->transform(tricky_matrix);
        break;
    }
    case ThreeDTypes::line:
    {
        primitives::Line* figure = reinterpret_cast<primitives::Line*>(obj);
        auto around = figure->center();
        auto shift1 = build_shift_matrix(around);
        auto scale = build_scale_matrix(vec);
        auto shift2 = build_shift_matrix(glm::vec3(0.0f));
        auto tricky_matrix = shift1 * scale * shift2;
        figure->transform(tricky_matrix);
        break;
    }
    default:
        break;
    }
}

void rotate_around_line(ThreeDInterface* obj, float angle, glm::vec3 p1, glm::vec3 p2)
{
    //матрица // какая матрица? Ты шиз?
    if (p2.z < p1.z || (p2.z == p1.z && p2.y < p1.y) ||
        (p2.z == p1.z && p2.y == p1.y) && p2.x < p1.x)
    {
        std::swap(p1, p2);
    }
    glm::vec3 center;
    switch (obj->type)
    {
    case ThreeDTypes::figure:
    {
        Figure* figure = reinterpret_cast<Figure*>(obj);
        center = figure->center();
        break;
    }
    case ThreeDTypes::rotation_figure:
    {
        RotationFigure* figure = reinterpret_cast<RotationFigure*>(obj);
        center = figure->center();
        break;
    }
    case ThreeDTypes::function_figure:
    {
        FunctionFigure* figure = reinterpret_cast<FunctionFigure*>(obj);
        center = figure->center();
        break;
    }
    case ThreeDTypes::line:
    {
        primitives::Line* figure = reinterpret_cast<primitives::Line*>(obj);
        center = figure->center();
        break;
    }
    default:
        break;
    }

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
    switch (obj->type)
    {
    case ThreeDTypes::figure:
    {
        Figure* figure = reinterpret_cast<Figure*>(obj);
        figure->transform(tricky_matrix);
        break;
    }
    case ThreeDTypes::rotation_figure:
    {
        RotationFigure* figure = reinterpret_cast<RotationFigure*>(obj);
        figure->transform(tricky_matrix);
        break;
    }
    case ThreeDTypes::function_figure:
    {
        FunctionFigure* figure = reinterpret_cast<FunctionFigure*>(obj);
        figure->transform(tricky_matrix);
        break;
    }
    case ThreeDTypes::line:
    {
        primitives::Line* figure = reinterpret_cast<primitives::Line*>(obj);
        figure->transform(tricky_matrix);
        break;
    }
    default:
        break;
    }

}

void rotate_around_center(ThreeDInterface* obj, Axis axis, float angle) {
    glm::vec3 around;
    switch (obj->type)
    {
    case ThreeDTypes::figure:
    {
        Figure* figure = reinterpret_cast<Figure*>(obj);
        around = figure->center();
        break;
    }
    case ThreeDTypes::rotation_figure:
    {
        RotationFigure* figure = reinterpret_cast<RotationFigure*>(obj);
        around = figure->center();
        break;
    }
    case ThreeDTypes::function_figure:
    {
        FunctionFigure* figure = reinterpret_cast<FunctionFigure*>(obj);
        around = figure->center();
        break;
    }
    case ThreeDTypes::line:
    {
        primitives::Line* figure = reinterpret_cast<primitives::Line*>(obj);
        around = figure->center();
        break;
    }
    default:
        break;
    }
    auto shift1 = build_shift_matrix(-around);
    auto rotate = build_rotation_matrix(axis, angle);
    auto shift2 = build_shift_matrix(around);
    auto tricky_matrix = shift2 * rotate * shift1;

    switch (obj->type)
    {
    case ThreeDTypes::figure:
    {
        Figure* figure = reinterpret_cast<Figure*>(obj);
        figure->transform(tricky_matrix);
        break;
    }
    case ThreeDTypes::rotation_figure:
    {
        RotationFigure* figure = reinterpret_cast<RotationFigure*>(obj);
        figure->transform(tricky_matrix);
        break;
    }
    case ThreeDTypes::function_figure:
    {
        FunctionFigure* figure = reinterpret_cast<FunctionFigure*>(obj);
        figure->transform(tricky_matrix);
        break;
    }
    case ThreeDTypes::line:
    {
        primitives::Line* figure = reinterpret_cast<primitives::Line*>(obj);
        figure->transform(tricky_matrix);
        break;
    }
    default:
        break;
    }

}

void reflection_about_the_axis(ThreeDInterface* obj, Axis axis)
{
    auto reflection_matrix = build_reflection_matrix(axis);
    switch (obj->type)
    {
    case ThreeDTypes::figure:
    {
        Figure* figure = reinterpret_cast<Figure*>(obj);
        figure->transform(reflection_matrix);;
        break;
    }
    case ThreeDTypes::rotation_figure:
    {
        RotationFigure* figure = reinterpret_cast<RotationFigure*>(obj);
        figure->transform(reflection_matrix);
        break;
    }
    case ThreeDTypes::function_figure:
    {
        FunctionFigure* figure = reinterpret_cast<FunctionFigure*>(obj);
        figure->transform(reflection_matrix);
        break;
    }
    case ThreeDTypes::line:
    {
        primitives::Line* figure = reinterpret_cast<primitives::Line*>(obj);
        figure->transform(reflection_matrix);
        break;
    }
    default:
        break;
    }

}