#pragma once
#include "ThreeDInterface.h"
#include <functional>
#include "3DChanger.h"

class RotationFigure : public HighLevelInterface {
    Line base;
public:
    RotationFigure(Line base) {
        this->base = base;
        this->objects = std::vector<Primitive>();
        this->objects.push_back(this->base);
        this->type = ThreeDTypes::rotation_figure;
    }

    void build(Axis axis, size_t partitions_count) {
        float step = 360.f / partitions_count;
        float angle = step;
        auto prev = base.copy();
        for (size_t i = 0; i < partitions_count; i++) {
            Line new_line = base.copy();
            rotate_line1(&new_line, axis, angle);
            for (size_t i = 0; i < new_line.points.size() - 1; i++) {
                Polygon poly;
                poly.color = base.color;
                poly.push_point(prev.points[i]);
                poly.push_point(prev.points[i + 1]);
                poly.push_point(new_line.points[i + 1]);
                poly.push_point(new_line.points[i]);
                poly.primitive_is_finished();

                objects.push_back(poly);
            }
            
            prev = new_line;
            angle += step;
        }
    }

    glm::vec3 center() {
        float x = 0, y = 0, z = 0;
        auto res = glm::vec3(0.0f);
        for (auto& prim : objects) {
            Line* line = reinterpret_cast<Line*>(&prim);
            res += line->center();
        }
        size_t size = objects.size();
        res /= size;
        return res;
    }

    void transform(const glm::mat4x4& transform_matrix) {
        for (size_t i = 0; i < objects.size(); i++) {
            Line* line = reinterpret_cast<Line*>(&objects[i]);
            line->transform(transform_matrix);
        }
    }
};