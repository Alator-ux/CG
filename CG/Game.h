#pragma once
#include "ObjModel.h"
#include "Camera.h"
class Game {
    // Gamer section
    glm::vec3 gamer_offset;
    Model gamer;
    // Scene section
    std::vector<Model> scene;
    DirectionLight light;
    // World section
    Camera camera;
    Shader shader;
    void move_camera(bool* keys) {
        if (keys[GLFW_KEY_F1]) {
            camera.Position = glm::vec3(0, 0, 3);
            camera.Yaw = -90;
            camera.updateCameraVectors();
            return;
        }
        if (keys[GLFW_KEY_W])
            camera.ProcessKeyboard(FORWARD);
        if (keys[GLFW_KEY_S])
            camera.ProcessKeyboard(BACKWARD);
        if (keys[GLFW_KEY_A])
            camera.ProcessKeyboard(LEFT);
        if (keys[GLFW_KEY_D])
            camera.ProcessKeyboard(RIGHT);
        if (keys[GLFW_KEY_Z])
            camera.ProcessKeyboard(DOWN);
        if (keys[GLFW_KEY_X])
            camera.ProcessKeyboard(UP);
        if (keys[GLFW_KEY_Q])
            camera.ProcessKeyboard(LEFT_ROTATE);
        if (keys[GLFW_KEY_E])
            camera.ProcessKeyboard(RIGHT_ROTATE);
        if (keys[GLFW_KEY_R])
            camera.ProcessKeyboard(DOWN_ROTATE);
        if (keys[GLFW_KEY_T])
            camera.ProcessKeyboard(UP_ROTATE);
    }
    void move_gamer() {
        gamer.model_matrix = glm::translate(glm::mat4(1.f), camera.Position + gamer_offset);
        gamer.model_matrix = glm::rotate(gamer.model_matrix, 
            -glm::radians(camera.Yaw + 180), glm::vec3(0.f, 1.f, 0.f));
    }
    void load_scene() {
        light.direction = glm::vec3(0.f, -1.f, 0.f);

        ObjTexture tank_tex("models/game/Tank.png", 'n');
        Material tank_mat(tank_tex);
        this->gamer = Model("models/game/Tanks.obj", tank_mat);

        ObjTexture field_tex("models/game/Field.png", 'n');
        Material field_mat(field_tex);
        this->scene.push_back(Model("models/game/Field.obj", field_mat));

    }
    void load_shader() {
        this->shader.init_shader("toon_shading.vert", "toon_shading.frag");
        shader.use_program();
        shader.uniformMatrix4fv("Projection", 
            glm::value_ptr(glm::perspective(glm::radians(45.f), 1.f, 0.1f, 1000.f)));
        shader.uniformMatrix4fv("View", glm::value_ptr(camera.GetViewMatrix()));
        shader.uniformMatrix4fv("Model", glm::value_ptr(glm::mat4(1.f)));
        shader.uniformDirectionLight(light, "dirLight.");
        shader.uniformMaterial(gamer.material, "material.");
        shader.disable_program();
    }
public:
    Game() {}
    void init() {
        this->gamer_offset = glm::vec3(-8.f, -4.0f, -0.0f);
        this->camera = Camera(-1.f*gamer_offset, glm::vec3(0.f, 1.f, 0.f), -180, -16);
        load_scene();
        load_shader();
    }
    void move(bool* keys) {
        move_camera(keys);
        move_gamer();
    }
    void render() {
        shader.use_program();
        shader.uniformMatrix4fv("View", glm::value_ptr(camera.GetViewMatrix()));
        shader.uniformMatrix4fv("Model", glm::value_ptr(gamer.model_matrix));
        gamer.render();
        for (auto& model : scene) {
            shader.uniformMatrix4fv("Model", glm::value_ptr(model.model_matrix));
            model.render();
        }
    }
};