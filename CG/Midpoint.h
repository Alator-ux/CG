#pragma once
#include "Primitives.h"
#include <vector>

class MidpointDisplacer {
    std::vector<Segment> Mountains;
    float gen_random() {
        return (float)rand() / RAND_MAX;
    }
public:
    MidpointDisplacer(std::vector<Segment> initial_lines) {
        this->Mountains = move(initial_lines);
    }
    void Build_mountain(float coefficient, size_t amount) {
        for (size_t i = 0; i < amount; i++) {
            std::vector<Segment> temp_mount;
            for (Segment s : Mountains) {
                float rnd = gen_random();
                auto left = s.left();
                auto right = s.right();
                auto x_del = abs(left.x - right.x);
                auto y_del = abs(left.y - right.y);
                double dest = sqrt(x_del * x_del + y_del * y_del);
                auto x_cent = (left.x + right.x) / 2;
                auto y = (left.y + right.y) / 2 + (rnd - 0.5) * coefficient * dest;
                glm::vec3 center = glm::vec3(x_cent, y ,1.0);
                temp_mount.push_back(Segment(left,center));
                temp_mount.push_back(Segment(center, right));
            }
            this->Mountains = move(temp_mount); // WARN: may explode, idk
        }

    }
};