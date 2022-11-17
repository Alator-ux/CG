#pragma once
#include <vector>
#include "Light.h"
#include "Primitives.h"

std::vector<glm::vec3> lambert(primitives::Polygon& object, LightSource& ls) {
    std::vector<glm::vec3> res;
    if (!object.calc_normal()) {
        return res;
    }
    glm::vec3 norm = glm::normalize(object.normal);

    for (size_t i = 0; i < object.points.size(); i++) {
        auto lightDir = glm::normalize(object.points[i] - ls.position);
        glm::vec3 color;
        if (object.colors.size() > 1) {
            color = object.colors[i];
        }
        else {
            color = object.colors[0];
        }
        color *= glm::clamp(glm::dot(norm, lightDir), 0.f, 1.f);
        res.push_back(color);
    }

    return res;
}