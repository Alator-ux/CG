#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <vector>
#include "Primitives.h"
enum class ThreeDTypes{figure, rotation_figure, function_figure, line};

class ThreeDInterface {
public:
    ThreeDTypes type;
    glm::vec3 center() {
        return glm::vec3(0);
    };
    void transform(const glm::mat4x4&) {

    };
};

class ThreeDPrimInterface : public ThreeDInterface, public Primitive{

};

class HighLevelInterface : public ThreeDInterface {
public:
    std::vector<Primitive> objects;
};