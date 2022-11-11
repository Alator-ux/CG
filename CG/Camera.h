#pragma once

// Std. Includes
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    LEFT_ROTATE,
    RIGHT_ROTATE,
    UP_ROTATE,
    DOWN_ROTATE
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 0.5f;

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    GLfloat Yaw;
    GLfloat Pitch;

    GLfloat MovementSpeed;
    GLfloat RotateSpeed = 1.0;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED)
    {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        auto f = glm::normalize(this->Front);
        auto s = glm::normalize(glm::cross(f, this->Up));
        auto u = glm::cross(s, f);

        auto res = glm::mat4x4(
            s.x, u.x, -f.x, 1,
            s.y, u.y, -f.y, 1,
            s.z, u.z, -f.z, 1,
            -glm::dot(s, this->Position), -glm::dot(u, this->Position), -glm::dot(f, this->Position), 1
        );

        auto mat1 = glm::mat4x4(
            Right.x, Right.y, Right.z, 0,
            Up.x, Up.y, Up.z, 0,
            Front.x, Front.y, Front.z, 0,
            0, 0, 0, 1
        );
        auto mat2 = glm::mat4x4(
            1, 0, 0, -Position.x,
            0, 1, 0, -Position.y,
            0, 0, 1, -Position.z,
            0, 0, 0, 1
        );
        //return mat1 * mat2;
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

    glm::vec3 toCameraView(glm::vec3 p) {
        return glm::vec3(Right.x * (p.x - Front.x) + Right.y * (p.y - Front.y) + Right.z * (p.z - Front.z),
            Up.x * (p.x - Position.x) + Up.y * (p.y - Position.y) + Up.z * (p.z - Position.z),
            Front.x * (p.x - Position.x) + Front.y * (p.y - Position.y) + Front.z * (p.z - Position.z));
    }

    void ProcessKeyboard(Camera_Movement direction)
    {
        std::cout << "Yaw=" << Yaw << "; Pitch=" << Pitch << "\n";
        GLfloat side_speed = this->MovementSpeed;// / 10;
        GLfloat velocity = this->MovementSpeed * 2;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * side_speed;
        if (direction == RIGHT)
            this->Position += this->Right * side_speed;
        if (direction == UP)
            this->Position += this->Up * side_speed;
        if (direction == DOWN)
            this->Position -= this->Up * side_speed;
        if (direction == LEFT_ROTATE) {
            this->Yaw += RotateSpeed;
            updateCameraVectors();
        }
        if (direction == RIGHT_ROTATE) {
            this->Yaw -= RotateSpeed;
            updateCameraVectors();
        }
        if (direction == UP_ROTATE) {
            this->Pitch += RotateSpeed;
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            else if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
            updateCameraVectors();
        }
        if (direction == DOWN_ROTATE) {
            this->Pitch -= RotateSpeed;
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            else if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
            updateCameraVectors();
        }
    }

    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->Front = glm::normalize(front);
        
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};