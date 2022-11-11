#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <cmath>
namespace projection {
    enum Type { pers, axon, none };

    glm::mat4x4 perspective(float fov, float aspect, float zNear, float zFar) {
        /*glm::mat4x4 res(0);
        auto half_tan = tan(fov / 2.0f);
        res[0][0] = 1.0f / (aspect * half_tan);
        res[1][1] = 1.0f / half_tan;
        res[2][2] = -(zFar + zNear) / (zFar - zNear);
        res[2][3] = -(2.0f * zFar * zNear) / (zFar - zNear);
        res[3][2] = -1.f;
        return res;*/
        /*glm::mat4x4 res(0);
        auto half_tan = tan(fov / 2.0f);
        res[0][0] = 1.0f / (aspect * half_tan);
        res[1][1] = 1.0f / half_tan;
        res[2][2] = -(zFar + zNear) / (zFar - zNear);
        res[2][3] = -1.0f;
        res[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
        return res;*/
        return glm::perspective(fov, aspect, zNear, zFar);
    }
    glm::mat4x4 perspective3(float fov, float width, float height, float zNear, float zFar) {
        return glm::mat4x4(
            height / (std::tan(glm::radians(fov / 2)) * width), 0, 0, 0,
            0, 1.0 / std::tan(glm::radians(fov / 2)), 0, 0,
            0, 0, -(zFar + zNear) / (zFar - zNear), -2 * (zFar * zNear) / (zFar - zNear),
            0, 0, -1, 0);
    }
    glm::mat4x4 perspective(float r = 0.005) {
        glm::mat4x4 res(1);
        res[2][3] = r;
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