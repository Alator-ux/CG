#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OpenGLWrappers.h"
#include "imgui.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "Widgets.h"
#include "Drawer.h"
#include "FigureBuilder.h"
#include "Camera.h"
#include "Texture.h"
#include <corecrt_math_defines.h>
#include "Cube.h"
const GLuint W_WIDTH = 600;
const GLuint W_HEIGHT = 600;
std::vector<Shader> shaders;
Drawer drawer;
std::vector<primitives::Primitive> storage;
Camera camera;
void Init(OpenGLManager*);
void Draw(OpenGLManager*, std::string&);
void Release();
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void Do_Movement();
int mode = 0;
glm::vec2 ab(1.f);
int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    glfwSetKeyCallback(window, keyboard_callback);
    const char* glsl_version = "#version 330";
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    glewInit();
    auto manager = OpenGLManager::get_instance();
    Init(manager);


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGui::StyleColorsDark();

    auto button_row = RadioButtonRow({ "C&T Cube", "T&T Cube", "Tetrahedron", "Ellipse"});

    bool show_demo_window = false;
    std::string vbo_name = "";
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        
        {
            ImGui::Begin("Window!");
            ImGui::Text("Current mode: " + mode);
            if (button_row.draw()) {
                vbo_name = button_row.get_label();
                mode = button_row.get_value();
            }
            ImGui::End();
        }


        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Do_Movement();
        Draw(manager, vbo_name);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

bool keys[1024];
float step = 0.005;
glm::mat4 el_scale(1.f);
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (mode == 3) {
        switch (key) {
        case GLFW_KEY_W:
        {
            el_scale = glm::scale(el_scale, glm::vec3(1.f, 1.01f, 1.f));
            ab[1] += step;
            break;
        }
        case GLFW_KEY_S: 
        {
            el_scale = glm::scale(el_scale, glm::vec3(1.f, 0.99f, 1.f));
            ab[1] -= step;
            break;
        }
        case GLFW_KEY_D:
        {
            el_scale = glm::scale(el_scale, glm::vec3(1.01, 1.f, 1.f));
            ab[0] += step;
            break;
        }
        case GLFW_KEY_A:
        {
            el_scale = glm::scale(el_scale, glm::vec3(0.99f, 1.f, 1.f));
            ab[0] -= step;
            break;
        }
        default:
            break;
        }
        shaders[mode].use_program();
        shaders[mode].uniformMatrix4fv("el_scale", el_scale);
        shaders[mode].uniform2f("ab", ab);
        shaders[mode].disable_program();
        return;
    }
    if (key == GLFW_KEY_F1) {
        camera.Position = glm::vec3(0, 0, 3);
        camera.Yaw = -90;
        camera.updateCameraVectors();
        return;
    }
    if (key >= 0 && key < 1024)
    {
        keys[key] = action == GLFW_PRESS;
    }

}
float mixCoef = 0.5;
void Do_Movement() {
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
    if (keys[GLFW_KEY_LEFT]) {
        mixCoef = glm::clamp((float)(mixCoef - 0.1), 0.f, 1.f);
        shaders[1].use_program();
        shaders[1].uniform1f("mixCoef", mixCoef);
        shaders[1].disable_program();

    }
    if (keys[GLFW_KEY_RIGHT]) {
        mixCoef = glm::clamp((float)(mixCoef + 0.1), 0.f, 1.f);
        shaders[1].use_program();
        shaders[1].uniform1f("mixCoef", mixCoef);
        shaders[1].disable_program();

    }
}

void Release() {
    OpenGLManager::get_instance()->release();
    for (auto& shader : shaders) {
        shader.release();
    }
}
primitives::Polygon poly;
void init_ellipse(OpenGLManager* manager) {
    float count = 102;
    float angle = M_PI * 2 / (count);
    /*for (int i = -1; i < 101; i++) {
        float x = sin(angle * i);
        float y = cos(angle * i);
        poly.push_point({ x,y,0.f });
    }*/
    poly.push_point({ 0.f,0.f,0.f }, { 1.f,1.f,1.f });
    float r = 1.0;
    float g = 0.0;
    float b = 0.0;
    float step = 1.0 / 34.0;
    for (int i = 0; i <= 33; i++) {
        float x = sin(angle * i);
        float y = cos(angle * i);
        std::cout << "r=" << r << " g=" << g << " b=" << b << "\n";
        poly.push_point({ x,y, 0.f }, { glm::max(r,0.f),g,b });
        r -= step;
        g += step;
    }
    r = 0.0;
    g = 1.0;
    for (int i = 34; i <= 67; i++) {
        float x = sin(angle * i);
        float y = cos(angle * i);
        std::cout << "r=" << r << " g=" << g << " b=" << b << "\n";
        poly.push_point({ x,y, 0.f }, { r,glm::max(g,0.f),b });
        g -= step;
        b += step;
    }
    g = 0.0;
    b = 1.0;
    for (int i = 68; i <= 101; i++) {
        float x = sin(angle * i);
        float y = cos(angle * i);
        std::cout << "r=" << r << " g=" << g << " b=" << b << "\n";
        poly.push_point({ x,y, 0.f }, { r,g,glm::max(b,0.f) });
        b -= step;
        r += step;
    }
    poly.push_point({ sin(0), cos(0),0.f}, {1.f,0.f,0.f});

    drawer.set_vbo("ellipse vPos", "ellipse vColors", { poly });
}
auto cube = FigureBuilder().buildCube(glm::vec3(1.f));
auto tetrahedron = FigureBuilder().buildTetrahedron(glm::vec3(1.f));
void Init(OpenGLManager* manager) {
    drawer = Drawer(W_WIDTH, W_HEIGHT);

    drawer.set_vbo("Cube Pos", "Cube Colors", "Cube Tex", { cube });

    drawer.set_vbo("Tetr Pos", "Tetr Colors", { tetrahedron });
    init_ellipse(manager);

    Shader tAcShader = Shader();
    tAcShader.init_shader("tAc.vert", "tAc.frag");
    shaders.push_back(tAcShader);

    auto tAtShader = Shader();
    tAtShader.init_shader("tAt.vert", "tAt.frag");
    shaders.push_back(tAtShader);

    auto gradShader = Shader();
    gradShader.init_shader("grad.vert", "grad.frag");
    shaders.push_back(gradShader);

    auto ellipseShader = Shader();
    ellipseShader.init_shader("ellipse.vert", "ellipse.frag");
    shaders.push_back(ellipseShader);

    auto view = camera.GetViewMatrix();
    for (size_t i = 0; i < shaders.size() - 1; i++) {
        shaders[i].use_program();
        shaders[i].uniformMatrix4fv("View", view);
        shaders[i].uniformMatrix4fv("Projection", glm::perspective(glm::radians(45.f), 1.f, 0.1f, 100.f));
        shaders[i].disable_program();
    }

    shaders[1].use_program();
    shaders[1].uniform1f("mixCoef", mixCoef);
    shaders[1].disable_program();

    shaders[shaders.size() - 1].use_program();
    shaders[shaders.size() - 1].uniformMatrix4fv("el_scale", el_scale);
    shaders[shaders.size() - 1].uniform2f("ab", ab);
    shaders[shaders.size() - 1].disable_program();

    CImgTexture tex1(W_WIDTH, W_HEIGHT, "./images/wood.jpg");
    manager->create_rgb16f_buffer("brick", tex1.get_width(), tex1.get_height(), tex1.image.data());

    CImgTexture tex2(W_WIDTH, W_HEIGHT, "./images/texture2.jpg");
    manager->create_rgb16f_buffer("idk", tex2.get_width(), tex2.get_height(), tex2.image.data());

    shaders[0].use_program();
    shaders[0].uniform1i("textur", 0);
    shaders[0].disable_program();
    shaders[1].use_program();
    shaders[1].uniform1i("text1", 0);
    shaders[1].uniform1i("text2", 1);
    shaders[1].disable_program();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    manager->checkOpenGLerror();
}

void Draw(OpenGLManager* manager, std::string& vbo_name) {
    switch (mode)
    {
    case 0:
    {
        auto view = camera.GetViewMatrix();
        shaders[mode].use_program();
        shaders[mode].uniformMatrix4fv("View", view);
        shaders[mode].disable_program();
        drawer.draw_tAc(&shaders[mode], cube);
        break;
    }
    case 1:
    {
        auto view = camera.GetViewMatrix();
        shaders[mode].use_program();
        shaders[mode].uniformMatrix4fv("View", view);
        shaders[mode].disable_program();
        drawer.draw_tAt(&shaders[mode], cube);
        break;
    }
    case 2:
    {
        auto view = camera.GetViewMatrix();
        shaders[mode].use_program();
        shaders[mode].uniformMatrix4fv("View", view);
        shaders[mode].disable_program();
        drawer.draw_tetr(&shaders[mode], tetrahedron);
        break;
    }
    case 3:
    {
        drawer.draw_ellipse(&shaders[mode], poly);
        break;
    }
    default:
        break;
    }
}