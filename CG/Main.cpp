#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OpenGLWrappers.h"
#include "imgui.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "Widgets.h"
#include "3DChanger.h"
#include "Drawer.h"
#include "FigureBuilder.h"
#include "FunctionFigure.h"
#include "Camera.h"
#include "Texture.h"
#include "TextureDrawer.h"
#include "FloatingHorizon.h"
const GLuint W_WIDTH = 600;
const GLuint W_HEIGHT = 600;
Shader mainShader;
Drawer drawer;
std::vector<primitives::Primitive> storage;
Camera camera;
void Init(OpenGLManager*);
void Draw(OpenGLManager*);
void Release();

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
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

    bool show_demo_window = false;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        Draw(manager);
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

void Release() {
    OpenGLManager::get_instance()->release();
    mainShader.release();
}


void InitTask1BO(OpenGLManager* manager)
{
    GLfloat a, b;
    a = 0.3;
    b = 0.4;
    primitives::Polygon prim;
    prim.push_point(glm::vec3(-a, b, 0.f));
    prim.push_point(glm::vec3(-a, -b, 0.f));
    prim.push_point(glm::vec3(a, -b, 0.f));
    prim.push_point(glm::vec3(a, b, 0.f));

    
    prim.drawing_type = GL_QUADS;
    prim.colors.push_back(glm::vec3(1.f, 0.f, 0.f));
    storage.push_back(prim);

    drawer.set_vbo("lab11", storage);

    manager->checkOpenGLerror();
}


void InitTask2BO(OpenGLManager* manager)
{
    primitives::Polygon prim;
    int angle_numbers = 5;
    float angle = 0;
    float radius = 0.45;
    float mult = PI / 180;
    prim.push_point(glm::vec3(0.0, -0.5, 0.0));
    for (int i = 0; i <= angle_numbers; i++) {
        glm::vec3 p = glm::vec3();
        p.x = radius * cos(angle * mult);
        p.y = radius * sin(angle * mult);
        angle += 180.0 / angle_numbers;
        prim.push_point(p);
    }
    prim.drawing_type = GL_TRIANGLE_FAN;
    prim.colors.push_back(glm::vec3(1.f, 0.f, 0.f));
    storage.push_back(prim);

    drawer.set_vbo("lab11", storage);

    manager->checkOpenGLerror();
}


// Правильный многоугольник
void InitTask3BO(OpenGLManager* manager)
{
    primitives::Polygon prim;
    int angle_numbers = 5;
    float angle = 0;
    float radius = 0.45;
    float mult = PI / 180;
    for (int i = 0; i <= angle_numbers; i++) {
        glm::vec3 p = glm::vec3();
        p.x = radius * cos(angle * mult);
        p.y = radius * sin(angle * mult);
        angle += 360.0 / angle_numbers;
        prim.push_point(p);
    }
    prim.drawing_type = GL_TRIANGLE_FAN;
    prim.colors.push_back(glm::vec3(1.f, 0.f, 0.f));
    storage.push_back(prim);

    drawer.set_vbo("lab11", storage);

    manager->checkOpenGLerror();
}



void InitBO(OpenGLManager* manager)
{    
    primitives::Polygon prim;
    prim.push_point(glm::vec3(-1.f, -1.f, 0.f));
    prim.push_point(glm::vec3(0.f, 1.f, 0.f));
    prim.push_point(glm::vec3(1.f, -1.f, 0.f));
    prim.drawing_type = GL_TRIANGLES;
    prim.colors.push_back(glm::vec3(1.f, 0.f, 0.f));
    storage.push_back(prim);
    
    drawer.set_vbo("triangle", storage);

    manager->checkOpenGLerror();
}


void Init(OpenGLManager* manager) {
    
    mainShader = Shader();
    mainShader.init_shader("main.vert", "main.frag");
    drawer = Drawer(&mainShader, "vPos", W_WIDTH, W_HEIGHT);
    InitTask1BO(manager);
    //InitTask2BO(manager);
    //InitTask3BO(manager);
    glClearColor(0, 0, 0, 1);
    manager->checkOpenGLerror();
}

void Draw(OpenGLManager* manager) {
    glLineWidth(1.0f);
    drawer.draw(storage, "lab11", camera);
}