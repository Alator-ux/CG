#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "3DChanger.h"
#include "Primitives.h"
#include "Camera.h"
class CameraV2
{

    void rotateVectors(glm::vec3& vector1, glm::vec3& vector2, float angle, const glm::vec3& axis) {
        double l = axis.x;
        double m = axis.y;
        double n = axis.z;
        double anglesin = std::sin(angle);
        double anglecos = std::cos(angle);
        glm::mat4x4 rotation = glm::mat4x4(l * l + anglecos * (1 - l * l), l * (1 - anglecos) * m - n * anglesin, l * (1 - anglecos) * n + m * anglesin, 0,
            l * (1 - anglecos) * m + n * anglesin, m * m + anglecos * (1 - m * m), m * (1 - anglecos) * n - l * anglesin, 0,
            l * (1 - anglecos) * n - m * anglesin, m * (1 - anglecos) * n + l * anglesin, n * n + anglecos * (1 - n * n), 0,
            0, 0, 0, 1);

        auto res = rotation * glm::vec4(vector1, 1);
        vector1 = glm::normalize(glm::vec3(res));
        res = rotation * glm::vec4(vector2, 1);
        vector2 = glm::normalize(glm::vec3(res));
    }

public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    const double cameraRotationSpeed = 1;
    double yaw = 0.0, pitch = 0.0;
    GLfloat movement_speed;
    GLfloat rotate_speed;
    CameraV2() {
        position = glm::vec3(0, 0, 2);
        front = glm::vec3(1, 0, 0);
        up = glm::vec3(0, 0, 1);
        right = glm::cross(front, up);
        movement_speed = 1.f;
        rotate_speed = 0.3f;
    }

    void move(float leftright = 0, float forwardbackward = 0, float updown = 0) {
        position += leftright * right + forwardbackward * front + updown * up;
    }
    void ProcessKeyboard(Camera_Movement direction) {
        if (direction == Camera_Movement::FORWARD)
            this->position += front * movement_speed;
        if (direction == Camera_Movement::BACKWARD)
            this->position -= front * movement_speed;
        if (direction == Camera_Movement::LEFT)
            this->position -= right * movement_speed;
        if (direction == Camera_Movement::RIGHT)
            this->position += right * movement_speed;
        if (direction == Camera_Movement::UP)
            this->position += up * movement_speed;
        if (direction == Camera_Movement::DOWN)
            this->position -= up * movement_speed;
        if (direction == Camera_Movement::LEFT_ROTATE) {
            changeView(-rotate_speed, 0.f);
        }
        if (direction == Camera_Movement::RIGHT_ROTATE) {
            changeView(rotate_speed, 0.f);
        }
        if (direction == Camera_Movement::UP_ROTATE) {
            changeView(0, rotate_speed);
        }
        if (direction == Camera_Movement::DOWN_ROTATE) {
            changeView(0, -rotate_speed);
        }
    }
    glm::mat4x4 GetViewMatrix() {
        auto uc = -glm::dot(up, position);
        auto vc = -glm::dot(right, position);
        auto nc = -glm::dot(front, position);
        return glm::mat4x4(
            up.x, right.x, front.x, 0,
            up.y, right.x, front.x, 0,
            up.z, right.x, front.z, 0,
            uc, vc, nc, 1
        );
    }

    void changeView(float shiftX = 0, float shiftY = 0)
    {
        float newPitch = glm::clamp(pitch + shiftY * cameraRotationSpeed, -89.0, 89.0);
        float newYaw = int(yaw + shiftX) % 360;
        if (newPitch != pitch)
        {

            rotateVectors(front, up, (newPitch - pitch), right);
            pitch = newPitch;
        }
        if (newYaw != yaw)
        {
            rotateVectors(front, right, (newYaw - yaw), up);
            yaw = newYaw;
        }
    }
};