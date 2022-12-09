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
#include "ObjModel.h"
const GLuint W_WIDTH = 600;
const GLuint W_HEIGHT = 600;
Shader shader;
Drawer drawer;
Camera camera;
void Init(OpenGLManager*);
void Draw(OpenGLManager*, std::string&);
void Release();
void rotate_system();
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void Do_Movement();
int mode = 0;

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
        rotate_system();

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
void Release() {
    OpenGLManager::get_instance()->release();
    shader.release();
}

std::vector<GLfloat> orbit_speeds = {0.f, 0.002f, 0.005f, 0.004f, 0.009f, 0.015f};
std::vector<GLfloat> self_speeds = {0.04f, 0.004f, 0.007f, 0.009f, 0.001f, 0.003f};
std::vector<glm::vec4> mutations =
{
        {0.f, 0.15f, 0.f, 0.f}, 
        {10.f, 1.f, 0.f, 0.f}, 
        {15.f, 1.f, 0.f, 0.f}, 
        {20.f, 1.f, 0.f, 0.f}, 
        {25.f, 1.f, 0.f, 0.f}, 
        {30.f, 1.f, 0.f, 0.f}, 
};
void rotate_system() {
    for (size_t i = 0; i < mutations.size(); i++)
    {
        mutations[i].z = fmod(mutations[i].z + self_speeds[i], 2 * M_PI);
        mutations[i].w = fmod(mutations[i].w + orbit_speeds[i], 2 * M_PI);
    }
}
Model statue1;
Model statue2;
void Init(OpenGLManager* manager) {
    drawer = Drawer(W_WIDTH, W_HEIGHT);
    
    shader.init_shader("sun_system.vert", "sun_system.frag");

    ObjTexture tex1("models/buddhist_statue/mesh_Model_5_u0_v0_diffuse.jpeg", 'n');
    statue1 = Model("./models/buddhist_statue/buddhist statue.obj", tex1);

    ObjTexture tex2("./models/buddha/buddha_head.jpg", 'n');
    statue2 = Model("./models/buddha/buddha_head_ma2.obj", tex2);
        //Model("./models/buddhist_statue/buddhist statue.obj",
        //"models/buddhist_statue/mesh_Model_5_u0_v0_diffuse.jpeg", true
        

    shader.use_program();
    auto mat = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 1000.f);
    shader.uniformMatrix4fv("Projection", glm::value_ptr(mat));
    mat = camera.GetViewMatrix();
    shader.uniformMatrix4fv("View", glm::value_ptr(mat));
    mat = glm::mat4(1.f);
    auto scaled = glm::scale(glm::mat4(1.f), glm::vec3(1.f));
    std::vector<glm::mat4> model = { mat, scaled };
    shader.uniformMatrix4fv("Model", glm::value_ptr(mat));
    shader.uniform1i("text", 0);
    shader.disable_program();


    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    manager->checkOpenGLerror();
}

void Draw(OpenGLManager* manager, std::string& vbo_name) {
    shader.use_program();
    shader.uniformMatrix4fv("View", glm::value_ptr(camera.GetViewMatrix()));
    shader.uniform4fv("mutations", glm::value_ptr(mutations[0]), mutations.size());
    shader.uniform1i("offset", 0);
    statue1.render();
    shader.uniform1i("offset", 1);
    statue2.render(5);
    shader.disable_program();
}