#pragma once
#include "Camera.h"

class GameCamera : public Camera {
public:
    glm::vec3 player_pos;
    glm::vec3 player_offset;
    GameCamera(glm::vec3 player_pos = glm::vec3(0.f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH)
    {
        this->player_pos = player_pos;
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
        this->MovementSpeed = SPEED;
        this->updateCameraVectors();
    }
    glm::mat4 GetViewMatrix() override {
        return glm::lookAt(this->Position, this->player_pos, this->Up);
    }
    void ProcessKeyboard(Camera_Movement direction) override
    {
        std::cout << "Yaw=" << Yaw << "; Pitch=" << Pitch << "\n";
        std::cout << "x=" << Position.x << " y=" << Position.y << " z=" << Position.z << "\n";
        GLfloat side_speed = this->MovementSpeed;// / 10;
        GLfloat velocity = this->MovementSpeed;

        switch (direction)
        {
        case FORWARD:
        {
            auto offset = this->Front * velocity;
            this->Position += offset;
            this->player_pos += offset;
            break;
        }
        case BACKWARD:
        {
            auto offset = this->Front * velocity;
            this->Position -= offset;
            this->player_pos -= offset;
            break;
        }
        case LEFT:
        {
            this->Yaw -= RotateSpeed;
            updateCameraVectors();
            break;
        }
        case RIGHT:
        {
            this->Yaw += RotateSpeed;
            updateCameraVectors();
            break;
        }
        case LEFT_ROTATE:
        {
            rotate_camera(RotateSpeed);
            break;
        }
        case RIGHT_ROTATE:
        {
            rotate_camera(-RotateSpeed);
            break;
        }
        default:
            break;
        }
    }
    void rotate_camera(GLfloat speed) {
        auto pos = glm::translate(glm::mat4(1.f), -player_pos) * glm::vec4(this->Position, 1.f);
        pos = glm::rotate(glm::mat4(1.f), glm::radians(speed), glm::vec3(0.f, 1.f, 0.f))
            * pos;
        this->Position = glm::translate(glm::mat4(1.f), player_pos) * pos;
    }
    void updateCameraVectors() override
    {
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw));
        front.y = 0.f;
        front.z = sin(glm::radians(this->Yaw));
        this->Front = glm::normalize(front);

        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};