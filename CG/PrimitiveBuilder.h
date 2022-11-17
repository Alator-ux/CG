#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>
#include <vector>
#include "Primitives.h"
class PrimitiveFabric {
    int code = 0;
    glm::vec3 color;
    std::vector<primitives::Primitive> primitives;
    bool prim_finished = true;
    size_t max_size;
public:
    PrimitiveFabric() {
        color = glm::vec3(1.0f);
        max_size = 10;
    }
    void update_code(int code) {
        if (this->code == code) {
            return;
        }
        this->code = code;
        if (!prim_finished) {
            primitives.erase(primitives.end() - 1);
        }
        prim_finished = true;
    }
    void update_color(glm::vec3 color) {
        this->color = color;
    }
    void build(glm::vec3 coords) {
        switch (code)
        {
        case 0:
            create_point(coords);
            break;
        case 1:
            if (prim_finished) {
                create_edge(coords);
            }
            else {
                primitives::Edge* edge = reinterpret_cast<primitives::Edge*>(&primitives[primitives.size() - 1]);
                edge->push_point(coords);
                prim_finished = edge->primitive_is_finished();
            }
            break;
        case 2:
            if (prim_finished) {
                create_polygon(coords);
            }
            else {
                primitives::Polygon* polygon = reinterpret_cast<primitives::Polygon*>(&primitives[primitives.size() - 1]);
                polygon->push_point(coords);
            }
        default:
            break;
        }
    }
    void finish_primitive() {
        switch (code)
        {
        case 0:
            prim_finished = true;
            break;
        case 1:
        {
            primitives::Edge* edge = reinterpret_cast<primitives::Edge*>(&primitives[primitives.size() - 1]);
            prim_finished = edge->primitive_is_finished();
            break;
        }
        case 2:
        {
            primitives::Polygon* polygon = reinterpret_cast<primitives::Polygon*>(&primitives[primitives.size() - 1]);
            prim_finished = polygon->primitive_is_finished();
            break;
        }
        default:
            break;
        }
    }
    void create_point(glm::vec3 coord) {
        if (primitives.size() == max_size) {
            return;
        }
        primitives.push_back(primitives::Point(coord, color));
        prim_finished = true;
    }
    void create_edge(glm::vec3 coords) {
        if (primitives.size() == max_size) {
            return;
        }
        primitives.push_back(primitives::Edge(coords, color));
        prim_finished = false;
    }
    void create_polygon(glm::vec3 coords) {
        if (primitives.size() == max_size) {
            return;
        }
        primitives.push_back(primitives::Polygon(coords, color));
        prim_finished = false;
    }
    void clear() {
        primitives.clear();
        prim_finished = true;
    }
    std::vector<primitives::Primitive>& get_items() {
        return primitives;
    }
    size_t size() {
        return primitives.size();
    }
    size_t get_max_size() {
        return max_size;
    }
};