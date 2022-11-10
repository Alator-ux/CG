#pragma once
#include "ThreeDInterface.h"
#include <functional>
#include "RotationFigure.h"

class FunctionFigure : public HighLevelInterface {
    void build(std::function<float(float, float)> f, int x0, int x1, int y0, int y1, int partition_count) {
        float xstep = float(std::abs(x0 - x1)) / float(partition_count);
        float ystep = float(std::abs(x0 - x1)) / float(partition_count);
        for (float x = x0; x < x1; x += xstep) {
            for (float y = y0; y < y1; y += ystep) {
                auto facex = primitives::Polygon();
                auto facey = primitives::Polygon();
                glm::vec3 p1 = glm::vec3(x, y, f(x, y)); // vanila
                glm::vec3 p2x = glm::vec3(x + xstep, y, f(x + xstep, y)); // x-shfited
                glm::vec3 p3 = glm::vec3(x + xstep, y + ystep, f(x + xstep, y + ystep)); // both-shifted
                glm::vec3 p2y = glm::vec3(x, y + ystep, f(x, y + ystep)); // y-shfited
                
                facex.push_point(p1);
                facey.push_point(p1);

                facex.push_point(p2x);
                facey.push_point(p3);

                facex.push_point(p3);
                facey.push_point(p2y);

                facex.primitive_is_finished();
                facey.primitive_is_finished();

                objects.push_back(facex);
                objects.push_back(facey);
            }
        }
        rotate_around_center(this, Axis::ox, 90);
    }
public:
    FunctionFigure(std::function<float(float, float)> f, int x0, int x1, int y0, int y1, int partition_count) {
        this->objects = std::vector<primitives::Primitive>();
        this->type = ThreeDTypes::rotation_figure;
        this->build(f, x0, x1, y0, y1, partition_count);
    }
    glm::vec3 center() {
        float x = 0, y = 0, z = 0;
        auto res = glm::vec3(0.0f);
        for (auto& prim : objects) {
            primitives::Line* line = reinterpret_cast<primitives::Line*>(&prim);
            res += line->center();
        }
        size_t size = objects.size();
        res /= size;
        return res;
    }
    void transform(const glm::mat4x4& transform_matrix) {
        for (size_t i = 0; i < objects.size(); i++) {
            primitives::Line* line = reinterpret_cast<primitives::Line*>(&objects[i]);
            line->transform(transform_matrix);
        }
    }
};