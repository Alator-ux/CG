#pragma once

#include "Primitives.h"
#include "PrimitiveChanger.h"
#include "GLM/glm.hpp"
//TODO: I'm not sure that this one is the best way to do the trick, but let it be
#define no_intersection glm::vec3(-1,-1,-1);

//const int empty_item = -1;
class LineWorker {
    Edge* f_edge;
    Edge* s_edge;

private:
    // Returns coefficients for line's function
    glm::vec3 get_line_function(Edge line) {
        auto p1 = line.points[0];
        auto p2 = line.points[1];
        GLfloat c1 = p1[1] - p2[1];
        GLfloat c2 = p2[0] - p1[0];
        GLfloat c3 = p1[0] * p2[1] - p2[0] * p1[1];
        return glm::vec3(c1, c2, c3);
    }
    bool point_on_line(GLfloat x, GLfloat y, Edge edge) {
        auto points = edge.points;
        bool res = x <= std::max(points[0].x, points[1].x) && x >= std::min(points[0].x, points[1].x);
        res = res && y <= std::max(points[0].y, points[1].y) && y >= std::min(points[0].y, points[1].y);
        
        return false;
    }
public:
    LineWorker() {
        this->f_edge = nullptr;
        this->s_edge = nullptr;
    }
    void push_edge(Primitive* item) {
        if (item == nullptr && item->type != 1) {
            return;
        }
        Edge* edge = reinterpret_cast<Edge*>(item);
        if (f_edge == nullptr) {
            f_edge = edge;
        }
        else if (s_edge == nullptr) {
            s_edge = edge;
        }
    }
    glm::vec3 find_intersection() {
        if (f_edge == nullptr || s_edge == nullptr) {
            return no_intersection;
        }
        auto res = this->find_intersection(*f_edge, *s_edge);
        f_edge = nullptr;
        s_edge = nullptr;
        return res;
    }
    glm::vec3 find_intersection(Edge first ,Edge second) {
        auto fc = this->get_line_function(first); // first line coefficients
        auto sc = this->get_line_function(second); // second line coefficients
        double delimiter = fc[0] * sc[1] - sc[0] * fc[1];
        if (delimiter == 0)
        {
            return no_intersection;
        }
        auto x = (fc[1] * sc[2] - sc[1] * fc[2]) / delimiter;
        auto y = (fc[2] * sc[0] - sc[2] * fc[0]) / delimiter;
        auto points = first.points;
        if (point_on_line(x, y, first) && point_on_line(x,y,second)) {
                return glm::vec3(x, y, 1);
        }
        return no_intersection;
    }
};