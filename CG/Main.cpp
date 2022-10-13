#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OpenGLWrappers.h"
#include "Primitives.h"
#include "Midpoint.h"
#include "imgui.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "Widgets.h"
#include "PrimitiveChanger.h"
#include "Selector.h"
#include "Fractal.h"
#include "Bezier.h"
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
MidpointDisplacer midp_disp = MidpointDisplacer(glm::vec3(W_WIDTH / 2, W_HEIGHT / 2, 0),
    glm::vec3(W_WIDTH, W_HEIGHT / 2, 0));
Bezier bezier;

void Init(OpenGLManager*);
void Draw(OpenGLManager*, int);
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
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::vector<string> items = { "Koch curve", "Koch snowflake",
        "Serpinsky Triangle", "Serpinsky carpet", "High tree"};
    auto ddm = DropDownMenu("Fractal type", items);
    auto lb = ListBox("Points", bezier.get_bezier_points());
    auto colorChooser = ColorChooser("Primitive Color");
    auto frac_slider = IntSlider("Fractal generation", 1, 15);
    auto fcscs = IntSlider("Color steps count", 0, 100); // frac color steps count slider
    auto frafs = IntSlider("Random angle from", 0, 360); // frac random angle from slider
    auto frats = IntSlider("Random angle to", 0, 360); // frac random angle to slider
    auto midp_count_slider = IntSlider("Midpoints count", 0, 40);
    auto midp_coef_slider = FloatSlider("Midpoints coef", 0, 1);
    auto rbr = RadioButtonRow();
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
            rbr.draw();
            switch (rbr.get_value())
            {
            case 0:
            {
                mode = -1;
                if (ddm.draw()) {
                    frac.set_fractal_type(ddm.selectedItem);
                }
                if (frac_slider.draw()) {
                    frac.set_generation(frac_slider.get_value());
                }
                /*if (fcscs.draw()) {
                    frac.set_color_steps(fcscs.get_value());
                }*/
                frafs.draw();
                frats.draw();
                if (ImGui::Button("Set start color")) {
                    frac.set_start_color(colorChooser.rgb_value());
                }
                ImGui::SameLine();
                if (ImGui::Button("Set end color")) {
                    frac.set_end_color(colorChooser.rgb_value());
                }
                ImGui::SameLine();
                if (ImGui::Button("Generate fractal")) {
                    frac.generate(frafs.get_value(), frats.get_value());
                    drawer.set_vbo("frac", frac.get_items());
                }
                break;
            }
            case 1:
            {
                mode = -1;
                if (midp_count_slider.draw()) {
                    midp_disp.set_count(midp_count_slider.get_value());
                }
                midp_coef_slider.draw();
                if (ImGui::Button("Next midpoint")) {
                    midp_disp.next(midp_coef_slider.get_value(), colorChooser.rgb_value());
                    drawer.set_vbo("midp", midp_disp.get_items());
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear")) {
                    midp_disp.clear();
                    drawer.set_vbo("midp", midp_disp.get_items());
                }
                ImGui::Text("Midpoints %i/%i", midp_disp.get_current_count(),
                    midp_disp.get_amount_count());
                break;
            }
            case 2:
            {
                if (ImGui::Button("Add point")) {
                    mode = 0;
                }
                ImGui::SameLine();
                if (ImGui::Button("Delete point")) {
                    bezier.delete_point();
                    drawer.set_vbo("bez_points", bezier.get_points());
                }
                ImGui::SameLine();
                if (ImGui::Button("Shift point")) {
                    mode = 1;
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear")) {
                    bezier.clear();;
                }
                if (ImGui::Button("Set line color")) {
                    bezier.set_line_color(colorChooser.rgb_value());
                }
                ImGui::SameLine();
                if (ImGui::Button("Set points color")) {
                    bezier.set_points_color(colorChooser.rgb_value());
                }
                ImGui::SameLine();
                if (ImGui::Button("Generate")) {
                    bezier.generate();
                    std::vector<Primitive> line;
                    auto l = bezier.get_line();
                    if (l != nullptr) {
                        line.push_back(*l);
                        drawer.set_vbo("bez_line", line);
                    }
                }
                lb.draw();
                bezier.set_active_point(lb.selectedItem);
            }
            default:
                break;
            }
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
        Draw(manager, rbr.get_value());
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
                bezier.add_point(glm::vec3(xpos, ypos, 1));
                drawer.set_vbo("bez_points", bezier.get_points());
                break;
            }
            case 1:
            {
                bezier.shift_point(glm::vec3(xpos, ypos, 1));
                drawer.set_vbo("bez_points", bezier.get_points());
                break;
            }
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
    drawer.set_vbo("midp", midp_disp.get_items());
    InitBO(manager);

    glPointSize(5);
    manager->checkOpenGLerror();
}

void Draw(OpenGLManager* manager, int mode) {
    glLineWidth(1.0f);
    switch (mode)
    {
    case 0:
    {
        drawer.draw(frac.get_items(), "frac");
        break;
    }
    case 1:
    {
        drawer.draw(midp_disp.get_items(), "midp");
        break;
    }
    case 2:
    {
        auto p = bezier.get_points();
        drawer.draw(p, "bez_points");
        p = std::vector<Primitive>();
        auto line = bezier.get_line();
        if (line == nullptr) {
            break;
        }
        p.push_back(*line);
        drawer.draw(p, "bez_line");
        break;
    }
    default:
        break;
    }
    
    //manager->unbind_framebuffer();
    //mainShader.disable_program();
}