#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OpenGLWrappers.h"
#include "Primitives.h"

#include "imgui.h" // necessary for ImGui::*, imgui-SFML.h doesn't include imgui.h

//#include "imgui-SFML.h" // for ImGui::SFML::* functions and SFML-specific overloads

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "Widgets.h"
#include "PrimitiveChanger.h"
#include "Line_worker.h"
#include "PointClassifier.h"
#include "Selector.h"
#include "Fractal.h"
using namespace std;

const GLuint W_WIDTH = 1280;
const GLuint W_HEIGHT = 720;
Shader mainShader;
Selector selector;
PrimitiveFabric pf;
Drawer drawer;
PrimitiveChanger pc;
Fractal frac = Fractal(0, 0, 0, 
    glm::vec3(W_WIDTH/2,W_HEIGHT/2,0), glm::vec3(W_WIDTH,W_HEIGHT,0));

void Init(OpenGLManager*);
void Draw(OpenGLManager*);
void Release();
void mouse_callback(GLFWwindow*, int, int, int);
void mouse_scrollback(GLFWwindow*, double, double);
glm::vec3 convert_coords(GLfloat, GLfloat, GLuint, GLuint);

int mode = -1;

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
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scrollback);
    glewInit();
    auto manager = OpenGLManager::get_instance();
    Init(manager);


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGui::StyleColorsDark();

    bool show_demo_window = true;
    bool show_another_window = false;
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::vector<string> items = { "Koch curve", "Koch snowflake",
        "Serpinsky Triangle", "Serpinsky carpet", "High tree"};
    auto lb = DropDownMenu("Fractal type", items);
    auto colorChooser = ColorChooser("Primitive Color");
    auto polygon_list = ListBox("Primitives", &pf.get_items());
    auto is = IntSlider("Fractal generation");
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        {
            ImGui::Begin("Hello, world!", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

            if (colorChooser.draw()) {
                pf.update_color(colorChooser.rgb_value());
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            auto cur_coords = convert_coords(xpos, ypos, W_WIDTH, W_HEIGHT);
            ImGui::Text("x = %.7f, y = %.7f", cur_coords.x, cur_coords.y);
            if (lb.draw()) {
                frac.set_fractal_type(lb.selectedItem);
                pf.update_code(lb.selectedItem);
            }
            if (is.draw()) {
                frac.set_generation(is.get_value());
            }
            std::stringstream ss;
            ss << "Mode " << mode;
            ImGui::Text(ss.str().c_str());
            if (ImGui::Button("Add")) {
                mode = 0;
            }
            ImGui::SameLine();
            if (ImGui::Button("Move")) {
                mode = 1;
            }
            ImGui::SameLine();
            if (ImGui::Button("Rotate")) {
                mode = 2;
            }
            ImGui::SameLine();
            if (ImGui::Button("Rotate 90")) {
                mode = 3;
            }
            ImGui::SameLine();
            if (ImGui::Button("Scale")) {
                mode = 4;
            }
            if (ImGui::Button("Classify")) {
                mode = 5;
            }
            ImGui::SameLine();
            if (ImGui::Button("Fractal")) {
                frac.generate();
                drawer.set_vbo("frac", frac.get_items());
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear")) {
                pf.clear();
                selector.set_item_index(-1);
            }
            if (polygon_list.draw()) {
                selector.set_item_index(polygon_list.selectedItem);
            }
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
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
}

glm::vec3 convert_coords(GLfloat x, GLfloat y, GLuint width, GLuint height) {
    /*x -= width / 2;
    x /= width / 2;
    y *= -1;
    y += height / 2;
    y /= height / 2;*/
    return glm::vec3((GLfloat)x, (GLfloat)y, 1.0f);
}

double oldx, oldy;
void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (xpos <= W_WIDTH / 2) {
            return;
        }
        std::cout << xpos << " " << ypos << std::endl;

 

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            switch (mode)
            {
            case 0: 
            {
                auto coords = convert_coords(xpos, ypos, W_WIDTH, W_HEIGHT);
                pf.build(coords);
                drawer.set_vbo("primitives", pf.get_items());
                break;
            }
            case 1:
            {
                auto coords = convert_coords(xpos, ypos, W_WIDTH, W_HEIGHT);
                pc.shift(selector.get_item(), coords);
                drawer.set_vbo("primitives", pf.get_items());
                break;
            }
            case 5:
            {
                auto coords = convert_coords(xpos, ypos, W_WIDTH, W_HEIGHT);
                //std::cout << pocl.classify(selector.get_item(), coords) << std::endl;
                break;
            }
            case 6:
            {
                break;
            }
            default:
                break;
            }

            oldx = xpos;
            oldy = ypos;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            switch (mode)
            {
            case 0:
                pf.finish_primitive();
            default:
                break;
            }
        }
    }
    
}

void mouse_scrollback(GLFWwindow* window, double xoffset, double yoffset) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << xoffset << " " << yoffset << std::endl;
    switch (mode)
    {
    case 2:
    {
        auto coords = convert_coords(xpos, ypos, W_WIDTH, W_HEIGHT);
        pc.rotate_around_point(selector.get_item(), coords, yoffset);
        drawer.set_vbo("primitives", pf.get_items());
        break;
    }
    case 3:
    {
        pc.rotate_90(selector.get_item());
        drawer.set_vbo("primitives", pf.get_items());
        break;
    }
    case 4:
    {
        auto coords = convert_coords(xpos, ypos, W_WIDTH, W_HEIGHT);
        if (yoffset > 0) {
            pc.scale_from_point(selector.get_item(), coords, 2, 2);
        }
        else {
            pc.scale_from_point(selector.get_item(), coords, 0.5, 0.5);
        }
        drawer.set_vbo("primitives", pf.get_items());
        break;
    }
    default:
        break;
    }
}


void InitBO(OpenGLManager* manager)
{    

    manager->gen_framebuffer("outputFB");
    manager->bind_framebuffer("outputFB");
    manager->create_rgba_buffer("outputCB", W_WIDTH, W_HEIGHT);
    manager->attach_colorbuffer("outputCB");
    manager->attach_renderbuffer("outputRB", W_WIDTH, W_HEIGHT);
    manager->unbind_framebuffer();
    manager->check_framebuffer_completeness();

    manager->init_colorbuffer("output", W_WIDTH, W_HEIGHT);
    

    manager->checkOpenGLerror();
}


void Init(OpenGLManager* manager) {
    mainShader = Shader();
    mainShader.init_shader("main.vert", "main.frag");

    pf = PrimitiveFabric();
    selector = Selector(&pf.get_items());
    pc = PrimitiveChanger();
    drawer = Drawer(&mainShader, "vPos", W_WIDTH, W_HEIGHT);
    InitBO(manager);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // белый цвет как цвет заливки
    //glViewport(0, 0, W_WIDTH, W_HEIGHT);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //auto c = 1.5;
    //glOrtho(-c * W_WIDTH / W_HEIGHT, c * W_WIDTH / W_HEIGHT,
    //        -c, c, -1.0, 1.0);
    //glOrtho(0, W_WIDTH, 0, W_HEIGHT, -1, 1);
    manager->checkOpenGLerror();
}

void Draw(OpenGLManager* manager) {
    glLineWidth(1.0f);
    drawer.draw(pf.get_items(), "primitives");
    drawer.draw(frac.get_items(), "frac");
    //manager->unbind_framebuffer();
    //mainShader.disable_program();
}