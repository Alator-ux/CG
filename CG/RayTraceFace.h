#pragma once
#include "RayTraceTools.h"
#include <GLM/glm.hpp>


class RFace : public RayTraceObjectInterface {
    glm::vec3 toSurface(glm::vec3 point) {
        auto diff = point - center;
        return glm::vec3(
            glm::dot(this->wVector, diff),
            glm::dot(this->hVector, diff),
            glm::dot(this->normale, diff)
        );
    }

    bool pointOnSurface(glm::vec3 point) {
        auto diff = point - center;
        return glm::abs(glm::dot(this->normale, diff)) < 1e-5;
    }

    bool fitsBounds(glm::vec3 p) {
        return glm::abs(p.x) <= this->width / 2 && glm::abs(p.y) <= this->height / 2;
    }
public:
    glm::vec3 center;
    glm::vec3 color;
    RMaterial material;

    float width, height;
    glm::vec3 normale;
    glm::vec3 hVector, wVector; // height and width vectors

    glm::vec3 get_center() {
        return this->center;
    }
    glm::vec3 get_color() {
        return this->color;
    }
    RMaterial get_material() {
        return this->material;
    };

    RFace(glm::vec3 center, glm::vec3 normale, glm::vec3 hV, float width, float height, glm::vec3 color, RMaterial material) {
        this->center = center;
        this->normale = normale;
        this->width = width;
        this->height = height;
        this->color = color;
        this->material = material;
        this->hVector = Rnormalize(hV);
        this->wVector = Rnormalize(hV * normale);
    }

    std::tuple<bool, glm::vec3, glm::vec3> find_intersection(glm::vec3 direction, glm::vec3 originPoint) override {
        if (this->pointOnSurface(originPoint)) {
            return { false, glm::vec3(), glm::vec3() };
        }

        float tn = glm::dot(this->normale, this->center) - glm::dot(this->normale, originPoint);
        if (glm::abs(tn) < 1e-5) { // line on surface
            return { false, glm::vec3(), glm::vec3() };
        }

        float td = glm::dot(this->normale, direction); // line parallel surface
        if (glm::abs(td) < 1e-5) {
            return { false, glm::vec3(), glm::vec3() };
        }

        glm::vec3 pointOnLine = (originPoint + direction) * (-tn / td); // point somwhere in the world
        // vector from POL have to be collinear, but if z is negative - they are directed vice versa
        if ((pointOnLine.z - originPoint.z) / direction.z < 0) {
            return { false, glm::vec3(), glm::vec3() };

        }
        glm::vec3 translatedPoint = toSurface(pointOnLine); // translated that point to local surface

        if (this->fitsBounds(translatedPoint)) {
            return { true, pointOnLine, normale };
        }
        return { false, glm::vec3(), glm::vec3() };

    };
};