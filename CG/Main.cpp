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
#include "TextureDrawer.h"
#include "RayTraceFigures.h"

const GLuint W_WIDTH = 600;
const GLuint W_HEIGHT = 600;
std::vector<Shader> shaders;
std::vector<primitives::Primitive> storage;
Camera camera;
CImgTexture canvas(512,512);
TextureDrawer drawer(&canvas);
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

        auto cube = RCube(
            { -2,-1,10 },
            6,
            { 210, 105, 30 }, // Chocolate
            RMaterial(40, 0.25, 0.7, 0.05, 0.3, 0.6)
        );
        auto sphere = RSphere(
            { 2,1,17 },
            4,
            { 143, 188, 143 }, // DarkSeaGreen
            RMaterial(10, 0.1, 0.85, 0.05, 0.1, 0.2)
        );
        auto room = RRoom(
            { 0,0,0 }, 20,
            RMaterial(0, 0, 0.9, 0.1, 0, 0),
            RMaterial(0, 0, 0.9, 0.1, 0, 0),
            RMaterial(0, 0, 0.9, 0.1, 0, 0),
            RMaterial(0, 0, 0.9, 0.1, 0, 0),
            RMaterial(0, 0, 0.9, 0.1, 0, 0),
            RMaterial(0, 0, 0.9, 0.1, 0, 0)
            );
        auto light = RLight({-7,7,0}, 0.75);
        auto camera = glm::vec3{ 0,0,0 };
        std::vector<RayTraceObjectInterface*> scene = { &cube, &sphere, &room };
        drawer.draw(scene,camera,light,60);

        /*ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        */
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
    
}

