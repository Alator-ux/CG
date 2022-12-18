#pragma once
#include "ObjModel.h"
#include "GameCamera.h"
template <typename T>
struct Random {
    /// <summary>
    /// Возвращает случайное число от 0.0 до 1.0 включительно
    /// </summary>
    static T random() {
        return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
    }
    /// <summary>
    /// Возвращает случайное число от 'from' до 'to' включительно
    /// </summary>
    static T random(T from, T to) {
        return from + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX) / static_cast<T>(to - from));
    }
};
class Game {
    // Gamer section
    Model gamer;
    std::vector<FlashLight> headlights;
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
        if (keys[GLFW_KEY_SPACE]) {
            camera.Position = camera.player_pos - camera.player_offset;
            camera.rotate_camera(-camera.Yaw + 180);
        }
        return moved;
    }
    void move_gamer() {
        auto rot = glm::rotate(glm::mat4(1.f),
            -glm::radians(camera.Yaw + 180), glm::vec3(0.f, 1.f, 0.f));
        auto tr = glm::translate(glm::mat4(1.f), camera.player_pos);
        gamer.model_matrix = tr * rot;
    }
    glm::vec3 gen_position(float scale_factor) {
        auto x = Random<float>::random(-20, 20) * scale_factor;
        auto z = Random<float>::random(-20, 20) * scale_factor;
        return glm::vec3(x, 0, z);
    }
    void load_scene() {
        float scale_factor = 1.f;
        light.direction = glm::vec3(0.f, -1.f, 0.f);

        ObjTexture tex("models/game/Tank.png", 'n');
        Material mat(tex);
        this->gamer = Model("models/game/Tanks.obj", mat);
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

        tex = ObjTexture("models/game/Field.png", 'n');
        mat = Material(tex);
        auto  model = Model("models/game/Field.obj", mat);
        model.model_matrix = glm::scale(model.model_matrix, glm::vec3(scale_factor));
        this->scene.push_back(model);

        tex = ObjTexture("models/game/ChristmasTree.png", 'n');
        mat = Material(tex);
        model = Model("models/game/ChristmasTree.obj", mat);
        model.model_matrix = glm::translate(model.model_matrix, gen_position(scale_factor));
        this->scene.push_back(model);

        tex = ObjTexture("models/game/Stone-1.png", 'n');
        mat = Material(tex);
        model = Model("models/game/Stone-1.obj", mat);
        model.model_matrix = glm::translate(model.model_matrix, gen_position(scale_factor));
        this->scene.push_back(model);

        model = Model("models/game/Stone-2.obj", mat);
        model.model_matrix = glm::translate(model.model_matrix, gen_position(scale_factor));
            this->scene.push_back(model);


    }
    void load_shader() {
        this->shader.init_shader("toon_shading.vert", "toon_shading.frag");
        shader.use_program();
        shader.uniformMatrix4fv("Projection", 
            glm::value_ptr(glm::perspective(glm::radians(45.f), 1.f, 0.1f, 1000.f)));
        shader.uniformMatrix4fv("View", glm::value_ptr(camera.GetViewMatrix()));
        shader.uniformMatrix4fv("Model", glm::value_ptr(glm::mat4(1.f)));
        shader.uniformDirectionLight(light, "dirLight.");
        shader.uniformFlashLight(headlights[0], "fLight1.");
        shader.uniformFlashLight(headlights[1], "fLight2.");
        shader.uniformMaterial(gamer.material, "material.");
        shader.disable_program();
    }
public:
    Game() {}
    void init() {
        auto gamer_offset = glm::vec3(-8.f, -4.0f, -0.0f);
        this->camera = GameCamera(glm::vec3(0.f), -gamer_offset, glm::vec3(0.f, 1.f, 0.f), -180, -16);
        this->camera.player_offset = gamer_offset;
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