#pragma once

#include "Primitives.h"
#include "PrimitiveChanger.h"
#include "GLM/glm.hpp"
//TODO: I'm not sure that this one is the best way to do the trick, but let it be
#define no_intersection glm::vec3(-1,-1,-1);

class LineWorker {
    Edge edge;
private:
    // Returns coefficients for line's function
    glm::vec3 get_line_function(Edge line) {
        auto p1 = line.points[0];
        auto p2 = line.points[1];
        int c1 = p1[1] - p2[1];
        int c2 = p2[0] - p1[0];
        int c3 = p1[0] * p2[1] - p2[0] * p1[1];
        return glm::vec3(c1, c2, c3);
    }
public:
    /*LineWorker(Edge edge) { //I DON'T KNOW BRO
        this->edge = edge;
    }
    */ 

    glm::vec3 find_intersection(Edge other) {
        return this->find_intersection(this->edge, other);
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
        return glm::vec3(x, y, 1
        );
    }
};