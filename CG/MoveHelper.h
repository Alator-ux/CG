#pragma once
#include "Camera.h"
#include "GLM/fwd.hpp"
#include "Ray.h"

class Bullet : public Model {
public:
    Ray ray;
    Bullet() {}
    Bullet(
        const char* objFile,
        Material mat
    ) {
        manager = OpenGLManager::get_instance();
        material = mat;
        hasTexture = true;
        center = glm::vec3(0.f);
        std::vector<ObjVertex> mesh = loadOBJ(objFile, center);
        radius = glm::distance(glm::vec3(0.f), center);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }
    glm::vec3 move() {
        return ray.move();
    }
    bool checkCollisions(const std::vector<Model>& scene) {
        for (auto model : scene) {
            auto models_dist = glm::distance(this->center, model.center);
            if (models_dist <= this->radius + model.radius) {
                return true;
            }
        }
        return false;
    }
};

class PlayerMH : public Camera {
public:
    glm::vec3 model_pos;
    PlayerMH(glm::vec3 model_pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
        this->MovementSpeed = SPEED;
        this->model_pos = model_pos;
        this->updateCameraVectors();
    }
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }
    void ProcessKeyboard(Camera_Movement direction)
    {
        if(direction == FORWARD)
        {
            auto shift = Front * MovementSpeed;
            this->Position += shift;
            this->model_pos += shift;
        }
        if (direction == BACKWARD)
        {
            auto shift = Front * MovementSpeed;
            this->Position -= shift;
            this->model_pos -= shift;
        }
        if (direction == LEFT_ROTATE)
        {
            this->Yaw -= RotateSpeed;
            updateCameraVectors();
            rotate_camera(-RotateSpeed);
        }
        if (direction == RIGHT_ROTATE)
        {
            this->Yaw += RotateSpeed;
            updateCameraVectors();
            rotate_camera(RotateSpeed);
        }
    }
    // ѕоворот камеры (вау), переносим в центр, делаем поворот, возвращаем в исходную позицию
    void rotate_camera(GLfloat speed) {
        auto toCenter = glm::translate(glm::mat4(1.f), -model_pos);
        auto rotate = glm::rotate(glm::mat4(1.f), -glm::radians(speed), glm::vec3(0.f, 1.f, 0.f));
        auto back = glm::translate(glm::mat4(1.f), model_pos);
        this->Position = back * rotate * toCenter * glm::vec4(Position, 1.f);
    }
    // 
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = 0; // ћы не хотим уйти под поле / взлететь
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->Front = glm::normalize(front);

        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};