#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <cmath>
namespace projection {
    enum Type { pers, axon, none };

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

    glm::mat4x4 axonometric(float fi, float psi) {
        auto sin_fi = sin(fi);
        auto cos_fi = cos(fi);
        auto sin_psi = sin(psi);
        auto cos_psi = cos(psi);
        glm::mat4x4 res = glm::mat4x4(
            cos_fi, sin_fi * sin_psi, 0, 0,
            0, cos_fi, 0, 0,
            sin_psi, -cos_psi * sin_fi, 0, 0,
            0, 0, 0, 1
        );
        return res;
    }
}