#pragma once
#include "GL/glew.h"
#include "Primitives.h"
#include <vector>

class MidpointDisplacer {
    Primitive start_seg;
    std::vector<Primitive> Mountains;
    size_t current = 0;
    size_t amount;
    float gen_random() {
        return (float)rand() / RAND_MAX;
    }
public:
    MidpointDisplacer(glm::vec3 start, glm::vec3 end, 
        glm::vec3 color = glm::vec3(1)) {
        start_seg = Segment(start, end, color);
        Mountains.push_back(start_seg);
        amount = 0;
    }
    void set_count(size_t amount) {
        this->amount = amount;
    }
    void clear() {
        Mountains.clear();
        Mountains.push_back(start_seg);
        current = 0;
    }
    size_t get_current_count() {
        return current;
    }
    size_t get_amount_count() {
        return amount;
    }
    void next(float coefficient, glm::vec3 color) {
        if (current >= amount) {
            clear();
            return;
        }
        std::vector<Primitive> temp_mount;
        for (Primitive& prim : Mountains) {
            Segment* s = reinterpret_cast<Segment*>(&prim);
            float rnd = gen_random();
            auto left = s->left();
            auto right = s->right();
            auto x_del = abs(left.x - right.x);
            auto y_del = abs(left.y - right.y);
            double dest = sqrt(x_del * x_del + y_del * y_del);
            auto x_cent = (left.x + right.x) / 2;
            auto y = (left.y + right.y) / 2 + (rnd - 0.5) * coefficient * dest;
            glm::vec3 center = glm::vec3(x_cent, y, 1.0);
            temp_mount.push_back(Segment(left, center, color));
            temp_mount.push_back(Segment(center, right, color));
        }
        this->Mountains = move(temp_mount);
        this->current += 1;
    }
    std::vector<Primitive>& get_items() {
        return Mountains;
    }
};