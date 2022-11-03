#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "3DChanger.h"
class Camera
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;
    const double cameraRotationSpeed = 1;
    double yaw = 0.0, pitch = 0.0;
public:
    Camera() {
        position = glm::vec3(0, 0, 2);
        direction = glm::vec3(0, 0, 0);
        up = glm::vec3(0, 0, 1);
        right = glm::normalize(direction * up);
    }

    void move(float leftright = 0, float forwardbackward = 0, float updown = 0) {
        position += leftright * right + forwardbackward * direction + updown * up;
    }

    glm::vec3 getViewMatrix() {
        auto eye = position;
        auto center = position + front;
        return glm::mat4x4(

        );
    }

    void changeView(float shiftX = 0, float shiftY = 0)
    {
        float newPitch = glm::clamp(pitch + shiftY * cameraRotationSpeed, -89.0, 89.0);
        float newYaw = int(yaw + shiftX) % 360;
        if (newPitch != pitch)
        {
            
            //AffineTransformations.rotateVectors(ref direction, ref up, (newPitch - pitch), right);
            pitch = newPitch;
        }
        if (newYaw != yaw)
        {
            //AffineTransformations.rotateVectors(ref direction, ref right, (newYaw - yaw), up);
            yaw = newYaw;
        }
    }
}
}