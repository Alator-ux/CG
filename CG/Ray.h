#pragma once
#include <glm/glm.hpp>
struct Ray {
    glm::vec3 start, direction;
    float distance;

    Ray(glm::vec3 st, glm::vec3 direction)
    {
        start = st;
        this->direction = glm::normalize(direction);
        this->distance = 0;
    }
    Ray() { }
    Ray(const Ray& r)
    {
        start = r.start;
        direction = r.direction;
        distance = r.distance;
    }
    glm::vec3 move() {
        distance += 0.05;
        return start + direction * distance;
    }
};