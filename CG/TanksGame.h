#pragma once
#include "ObjModel.h"
#include "Camera.h"
#include <vector>

class TanksGame {
    Shader shader;
    Model tank;
    Camera camera;
    std::vector<Model> scene;
    PointLight sun;
    glm::vec3 pos;
    GLfloat speed;
    FlashLight headlightL;
    FlashLight headlightR;
    glm::vec3 cameraOffset;
    glm::vec3 lighOffset;

    bool moveCamera(bool* keys) {
        if (keys[GLFW_KEY_W]) {
            camera.ProcessKeyboard(FORWARD);
        }
        if (keys[GLFW_KEY_S]) {
            camera.ProcessKeyboard(BACKWARD);
        }
        if (keys[GLFW_KEY_A]) {
            camera.ProcessKeyboard(LEFT_ROTATE);
        }
        if (keys[GLFW_KEY_D]) {
            camera.ProcessKeyboard(RIGHT_ROTATE);
        }
        this->pos = camera.Position;
        return false;
    }

    void moveTank(bool* keys) {
        if (!moveCamera(keys)) {
            return;
        }
        tank.mm = glm::translate(glm::mat4(1.f), camera.Position + this->cameraOffset);
        moveHeadlights();
    }

    void moveHeadlights() {
    }

    void buildCamera() {
        this->camera = Camera({ 0.f, 1.f, 0.f });
        this->camera.MovementSpeed = speed;
    }
    void buildTank() {
        ObjTexture tankTexture("images/WOT/Tank.png", 'n');
        tank = Model("models/WOT/Tanks.obj", tankTexture);
        headlightL = FlashLight();
        headlightR = FlashLight({ 0.4, 0, -0.5 });

    }
    void buildScene() {
        ObjTexture fieldTexture("images/WOT/Field.png", 'n');
        auto field = Model("models/WOT/Tanks.obj", fieldTexture);
        ObjTexture christTreeTexture("images/WOT/ChristmassTree.png", 'n');
        auto christTree = Model("models/WOT/ChristmassTree.obj", christTreeTexture);
        christTree.mm = glm::translate(glm::mat4(1.f), { -10, 0, 4 });

        ObjTexture barrelTexture("images/WOT/Barrel.png", 'n');
        auto barrel = Model("models/WOT/Barrel.obj", barrelTexture);
        barrel.mm = glm::translate(glm::mat4(1.f), { 4, 0, -10 });
        ObjTexture stone("images/WOT/Stone-1.png", 'n');
        auto stone1 = Model("models/WOT/Stone-1.obj", stone);
        auto stone2 = Model("models/WOT/Stone-2.obj", stone);
        stone1.mm = glm::translate(glm::mat4(1.f), { -6, 0, 4 });
        stone2.mm = glm::translate(glm::mat4(1.f), { -6, 0, -4 });
        ObjTexture treeTex("images/WOT/Tree.png", 'n');
        auto tree = Model("models/WOT/Tree.obj", treeTex);
        tree.mm = glm::translate(glm::mat4(1.f), {10, 0, 4});
        this->scene = std::vector<Model>{ field, christTree, barrel, stone1, stone2, tree };
    }

public:
    TanksGame(GLfloat speed){
        this->speed = speed;
    }

    void render() {
        shader.use_program();
        shader.uniformMatrix4fv("View", glm::value_ptr(camera.Position));
        shader.uniformMatrix4fv("Model", glm::value_ptr(tank.mm));

        tank.render();
        for (auto elem : scene) {
            shader.uniformMatrix4fv("Model", glm::value_ptr(elem.mm));
            elem.render();
        }
    }
};
