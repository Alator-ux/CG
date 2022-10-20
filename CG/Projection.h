#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <cmath>
namespace projection {
    enum type { pers };

    glm::mat4x4 perspective(float fov, float aspect, float zNear, float zFar) {
        glm::mat4x4 res(0);
        auto half_tan = tan(fov / 2.0f);
        res[0][0] = 1.0f / (aspect * half_tan);
        res[1][1] = 1.0f / half_tan;
        res[2][2] = -(zFar + zNear) / (zFar - zNear);
        res[2][3] = -1.0f;
        res[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
        return res;
    }
}