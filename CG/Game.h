#pragma once
#include "ObjModel.h"
#include "GameCamera.h"
class Game {
    // Gamer section
    glm::vec3 gamer_offset;
    glm::vec3 start_gamer_offset;
    glm::vec3 gamer_pos;
    glm::vec3 start_gamer_pos;
    Model gamer;
    // Scene section
    std::vector<Model> scene;
    DirectionLight light;
    // World section
    GameCamera camera;
    Shader shader;
    bool move_camera(bool* keys) {
        if (keys[GLFW_KEY_F1]) {
            camera.Position = glm::vec3(0, 0, 3);
            camera.Yaw = -90;
            camera.updateCameraVectors();
            return false;
        }
        bool moved = false;
        if (keys[GLFW_KEY_W]) {
            camera.ProcessKeyboard(FORWARD);
            moved = true;
        }
        if (keys[GLFW_KEY_S]) {
            camera.ProcessKeyboard(BACKWARD);
            moved = true;
        }
        if (keys[GLFW_KEY_A]) {
            camera.ProcessKeyboard(LEFT);
            moved = true;
        }
        if (keys[GLFW_KEY_D]) {
            camera.ProcessKeyboard(RIGHT);
            moved = true;
        }
        if (keys[GLFW_KEY_Q]) {
            camera.ProcessKeyboard(LEFT_ROTATE);
            moved = true;
        }
        if (keys[GLFW_KEY_E]) {
            camera.ProcessKeyboard(RIGHT_ROTATE);
            moved = true;
        }
        return moved;
    }
    void move_gamer() {
        gamer_offset = start_gamer_offset;
        auto rot = glm::rotate(glm::mat4(1.f),
            -glm::radians(camera.Yaw + 180), glm::vec3(0.f, 1.f, 0.f));
        auto tr = glm::translate(glm::mat4(1.f), camera.player_pos);
        gamer.model_matrix = tr * rot;

        gamer_offset = glm::rotate(glm::mat4(1.f),
            glm::radians(camera.Yaw + 180), glm::vec3(0.f, 1.f, 0.f))
            * glm::vec4(gamer_offset, 1.f);

        
    }
    void load_scene() {
        light.direction = glm::vec3(-5.f, -1.f, 0.f);

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
        this->start_gamer_offset = glm::vec3(-8.f, -4.0f, -0.0f);
        this->camera = GameCamera(glm::vec3(0.f), -1.f * start_gamer_offset, glm::vec3(0.f, 1.f, 0.f), -180, -16);
        this->start_gamer_pos = glm::vec3(0.f);
        load_scene();
        load_shader();
    }
    void move(bool* keys) {
        bool moved = move_camera(keys);
        if (moved) {
            move_gamer();
        }
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