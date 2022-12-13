#pragma once
#include "ObjModel.h"
#include "GameCamera.h"
class Game {
    // Gamer section
    Model gamer;
    std::vector<FlashLight> headlights;
    // Scene section
    std::vector<Model> scene;
    PointLight light;
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
        auto rot = glm::rotate(glm::mat4(1.f),
            -glm::radians(camera.Yaw + 180), glm::vec3(0.f, 1.f, 0.f));
        auto tr = glm::translate(glm::mat4(1.f), camera.player_pos);
        gamer.model_matrix = tr * rot;
    }
    void load_scene() {
        light.position = glm::vec3(0.f, 100.f, 0.f);

        ObjTexture tank_tex("models/game/Tank.png", 'n');
        Material tank_mat(tank_tex);
        this->gamer = Model("models/game/Tanks.obj", tank_mat);
        auto l_headlight = FlashLight();
        l_headlight.position = glm::vec3(-1.f, 1.f, 1.f);
        l_headlight.direction = glm::vec3(-2.f, 2.f, 1.f);
        l_headlight.diffuse = glm::vec3(255 / 255, 207 / 255, 64 / 255);
        l_headlight.specular = glm::vec3(243 / 255, 218 / 255, 11 / 255);
        l_headlight.cutOff = 12.5f;
        auto r_headlight = FlashLight(l_headlight);
        r_headlight.position = glm::vec3(-1.f, 1.f, -1.f);
        r_headlight.direction = glm::vec3(-2.f, 2.f, -1.f);
        headlights.push_back(l_headlight);
        headlights.push_back(r_headlight);

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
        shader.uniformPointLight(light, "pLight.");
        shader.uniformFlashLight(headlights[0], "fLight1.");
        shader.uniformFlashLight(headlights[1], "fLight2.");
        shader.uniformMaterial(gamer.material, "material.");
        shader.disable_program();
    }
public:
    Game() {}
    void init() {
        auto gamer_offset = glm::vec3(-8.f, -4.0f, -0.0f);
        this->camera = GameCamera(glm::vec3(0.f), -1.f * gamer_offset, glm::vec3(0.f, 1.f, 0.f), -180, -16);
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
        auto position = glm::vec3(gamer.model_matrix * glm::vec4(headlights[0].position, 1.f));
        shader.uniform3f("fLight1.pos", position);
        shader.uniform3f("fLight1.direction", camera.Front);
        position = glm::vec3(gamer.model_matrix * glm::vec4(headlights[1].position, 1.f));
        shader.uniform3f("fLight2.pos", position);
        shader.uniform3f("fLight2.direction", camera.Front);
        shader.uniformMatrix4fv("View", glm::value_ptr(camera.GetViewMatrix()));
        shader.uniformMatrix4fv("Model", glm::value_ptr(gamer.model_matrix));
        gamer.render();
        for (auto& model : scene) {
            shader.uniformMatrix4fv("Model", glm::value_ptr(model.model_matrix));
            model.render();
        }
    }
};