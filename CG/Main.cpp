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
std::vector<Shader> shaders;
Shader lampShader;
Drawer drawer;
Camera camera;
void Init(OpenGLManager*);
void Draw(OpenGLManager*, int);
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

    auto button_row = RadioButtonRow({ "Phong", "Toon shading", "Rim"});

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
        Draw(manager, button_row.get_value());
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
}

std::vector<GLfloat> orbit_speeds = {0.f, 0.002f, 0.005f, 0.004f, 0.009f, 0.015f};
std::vector<GLfloat> self_speeds = {0.04f, 0.004f, 0.007f, 0.009f, 0.001f, 0.003f};
std::vector<glm::vec4> shifts = 
{
        {0.f, 1.0f, 0.f, 0.f}, 
        {10.f, 0.003504f, 0.f, 0.f}, 
        {15.f, 0.008691f, 0.f, 0.f}, 
        {20.f, 0.009149f, 0.f, 0.f}, 
        {25.f, 0.004868f, 0.f, 0.f}, 
        {30.f, 0.100398f, 0.f, 0.f}, 
};
void rotate_system() {
    for (size_t i = 0; i < shifts.size(); i++)
    {
        shifts[i].z = fmod(shifts[i].z + self_speeds[i], 2 * M_PI);
        shifts[i].w = fmod(shifts[i].w + orbit_speeds[i], 2 * M_PI);
    }
}
Model statue1;
Model statue2;
Model cube;
void Init(OpenGLManager* manager) {
    drawer = Drawer(W_WIDTH, W_HEIGHT);
    
    Shader shader = Shader();
    shader.init_shader("phong.vert", "phong.frag");
    shaders.push_back(shader);

    Shader shader1 = Shader();
    shader1.init_shader("toon_shading.vert", "toon_shading.frag");
    shaders.push_back(shader1);

    Shader shader2 = Shader();
    shader2.init_shader("rim.vert", "rim.frag");
    shaders.push_back(shader2);

    lampShader.init_shader("lamp.vert", "lamp.frag");
    
    cube = Model("./models/cube/Cube.obj");
    ObjTexture tex1("models/buddhist_statue/mesh_Model_5_u0_v0_diffuse.jpeg", 'n');
    Material mat1(tex1);
    mat1.shininess = 2.f;
    statue1 = Model("./models/buddhist_statue/buddhist statue.obj", mat1);

    //ObjTexture tex2("./models/buddha/buddha_head.jpg", 'n');
    //Material mat2(tex2);
    //statue2 = Model("./models/buddha/buddha_head_ma2.obj", mat2);
        //Model("./models/buddhist_statue/buddhist statue.obj",
        //"models/buddhist_statue/mesh_Model_5_u0_v0_diffuse.jpeg", true

    PointLight pLight(glm::vec3(5.0f, 10.0f, 15.0f));;
    pLight.position = { 0, 15, 15 };
    pLight.set_atten_zero();

    DirectionLight dirLight(glm::vec3(0.f, -1.f, 0.f));

    FlashLight flashLight(glm::vec3(-5.0f, 26.0f, -16.0f));;
    flashLight.direction = camera.Front;
    flashLight.diffuse = { 1.0, 0.0, 0.0 };
    flashLight.specular = { 1.0, 0.0, 0.0 };
    flashLight.cutOff = 12.5f;
    pLight.set_atten_zero();

    auto projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 1000.f);
    auto view = camera.GetViewMatrix();
    auto model = glm::mat4(1.f);
    {
        for (auto shader : shaders) {
            shader.use_program();
            shader.uniformMatrix4fv("Projection", glm::value_ptr(projection));
            shader.uniformMatrix4fv("View", glm::value_ptr(view));
            shader.uniformMatrix4fv("Model", glm::value_ptr(model));
            shader.uniformPointLight(pLight, "pLight.");
            shader.uniformDirectionLight(dirLight, "dirLight.");
            shader.uniformFlashLight(flashLight, "flashLight.");
            shader.uniformMaterial(mat1, "material.");
            shader.disable_program();
        }
    }
    {
        lampShader.use_program();
        lampShader.uniformMatrix4fv("Projection", glm::value_ptr(projection));
        lampShader.uniformMatrix4fv("View", glm::value_ptr(view));
        auto lampLoc = glm::translate(model, pLight.position);
        lampLoc = glm::scale(lampLoc, glm::vec3(1.f));
        lampShader.uniformMatrix4fv("Model", glm::value_ptr(lampLoc));
        lampShader.disable_program();
    }


    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    manager->checkOpenGLerror();
}

void Draw(OpenGLManager* manager, int n) {
    shaders[n].use_program();
    shaders[n].uniformMatrix4fv("View", glm::value_ptr(camera.GetViewMatrix()));
    //phongShader.uniform3f("viewPos", camera.Position);
    statue1.render();
    shaders[n].disable_program();

    lampShader.use_program();
    lampShader.uniformMatrix4fv("View", glm::value_ptr(camera.GetViewMatrix()));
    cube.render(1,GL_QUADS);
    lampShader.disable_program();
}