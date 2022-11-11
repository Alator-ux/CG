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
#include "Projection.h"
#include "FigureBuilder.h"
#include "FunctionFigure.h"
#include "Camera.h"
#include "Texture.h"
#include "TextureDrawer.h"

const GLuint W_WIDTH = 1280;
const GLuint W_HEIGHT = 1280;
Shader mainShader;
Drawer drawer;
std::vector<HighLevelInterface> storage;
Camera camera;
void Init(OpenGLManager*);
void Draw(OpenGLManager*, int);
void Release();
void Do_Movement();
void keyboard_callback(GLFWwindow*, int, int, int, int);
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
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scrollback);
    glewInit();
    auto manager = OpenGLManager::get_instance();
    Init(manager);


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGui::StyleColorsDark();

    bool show_demo_window = false;
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::vector<std::string> items = { "Perspective", "Axonometric", "None"};
    auto ddm = DropDownMenu("Projection type", items);
    //auto lb = ListBox("Points", bezier.get_bezier_points());
    auto colorChooser = ColorChooser("Primitive Color");
    

    
    auto fig_builder = FigureBuilder();

    items = {"Add", "Shift", "Scale", "Rotate", "Custom rotate", "Reflect by axis"};
    auto rbr = RadioButtonRow(items);

    // Widgets for Add mode
    items = {"Tetrahedron", "Octahedron", "Hexahedron", "Icosahedron", "Dodec", "Rotation figure","Function"};
    auto poly_menu = DropDownMenu("Polyhedron type", items);

    // Widgets for Shift and Scale mode
    auto vec3_selector = Vec3Selector();

    // Widgets for Scale mode
    auto scale_coef = InputFloat("Scale coef");

    // Widhets for Rotate mode
    items = { "ox", "oy", "oz" };
    auto axis_menu = DropDownMenu("Axis type", items, 80);
    auto angle_selector = InputFloat("Angle");


    
    /* Widgets for Custom rotate mode */
    // point 1 vec3 custom rotate selector
    auto p1_vec3_crs = Vec3Selector();
    // point 2 vec3 custom rotate selector
    auto p2_vec3_crs = Vec3Selector();


    // Widgets for Reflection relative to the selected coordinate plane
    // axis_menu
    CImgTexture tex(W_WIDTH / 2, W_HEIGHT / 2);
    TextureDrawer tex_drawer(&tex);
    primitives::Polygon poly(glm::vec3(200, 200, 0), glm::vec3(0));
    poly.push_point(glm::vec3(100, 50, 0));
    poly.push_point(glm::vec3(300, 400, 0));
    tex_drawer.set_projection_mode(projection::pers);
    cimg_library::CImgDisplay main_disp(tex.image, "Click a point");
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

            if (ddm.draw()) {
                tex_drawer.set_projection_mode(projection::Type(ddm.selectedItem));
                drawer.set_projection_mode(projection::Type(ddm.selectedItem));
            }
            main_disp.display(tex.image);
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
                poly_menu.draw();
                if (ImGui::Button("Apply")) {
                    storage.clear();
                    if (poly_menu.selectedItem == 5) {
                        auto func = [](float x, float y) {
                            return std::pow((x), 2) + std::pow((y), 2);
                        };
                        auto base = primitives::Line(glm::vec3(-1.f, -2.5f, 0.f), colorChooser.rgb_value());
                        base.push_point(glm::vec3(-2.f, 1.f, 0.f));
                        base.push_point(glm::vec3(-1.0f, 2.5f, 0.f));
                        base.push_point(glm::vec3(1.0f, 4.5f, 0.f));
                        auto fig = RotationFigure(base);
                        fig.build(Axis::oy, 90);
                        auto pointer = reinterpret_cast<HighLevelInterface*>(&fig);
                        //STL::save_to_file("./models/model.stl", *pointer);
                        //auto model = STL::load_from_file("./models/model.stl");
                        storage.push_back(fig);
                        drawer.set_vbo("figure", storage);
                        break;
                    }
                    if (poly_menu.selectedItem == 6) {
                        auto func = [](float x, float y) {
                            return std::pow((x), 2) + std::pow((y), 2);
                        };
                        auto fig = FunctionFigure(func, -1, 1, -1, 1, 10);
                        auto pointer = reinterpret_cast<HighLevelInterface*>(&fig);
                        STL::save_to_file("./models/model.stl", *pointer);
                        auto model = STL::load_from_file("./models/model.stl");
                        storage.push_back(model);
                        drawer.set_vbo("figure", storage);
                    }
                    else {
                        auto fig = fig_builder.buildFigure(FigureType(poly_menu.selectedItem),
                            colorChooser.rgb_value());
                        auto pointer = reinterpret_cast<HighLevelInterface*>(&fig);
                        storage.push_back(*pointer);
                        drawer.set_vbo("figure", storage);
                    }
                }
                break;
            }
            case 1:
            {
                vec3_selector.draw();
                if (ImGui::Button("Apply")) {
                    shift(&storage[0], vec3_selector.get_value());
                    drawer.set_vbo("figure", storage);
                }
                break;
            }
            case 2:
            {
                vec3_selector.draw();
                if (ImGui::Button("Apply")) {
                    scale(&storage[0], vec3_selector.get_value());
                    drawer.set_vbo("figure", storage);
                }
                ImGui::SameLine();
                ImGui::Text(" or ");
                ImGui::SameLine();
                if (ImGui::Button("Apply around center")) {
                    scale_around_center(&storage[0], vec3_selector.get_value());
                    drawer.set_vbo("figure", storage);
                }
                break;
            }
            case 3: 
            {
                axis_menu.draw();
                ImGui::SameLine();
                angle_selector.draw();
                if (ImGui::Button("Apply")) {
                    rotate(&storage[0], Axis(axis_menu.selectedItem), angle_selector.get_value());
                    drawer.set_vbo("figure", storage);
                }
                ImGui::SameLine();
                ImGui::Text(" or ");
                ImGui::SameLine();
                if (ImGui::Button("Apply around center")) {
                    rotate_around_center(&storage[0], Axis(axis_menu.selectedItem), angle_selector.get_value());
                    drawer.set_vbo("figure", storage);
                }
                break;
            }
            case 4:
            {
                ImGui::Text("First point coordinates: ");
                ImGui::SameLine();
                p1_vec3_crs.draw();
                ImGui::Text("Second point coordinates: ");
                ImGui::SameLine();
                p2_vec3_crs.draw();
                angle_selector.draw();
                if (ImGui::Button("Apply")) {
                    rotate_around_line(&storage[0], angle_selector.get_value(),
                        p1_vec3_crs.get_value(), p2_vec3_crs.get_value());
                    drawer.set_vbo("figure", storage);
                }
                break;
            }
            case 5:
            {
                axis_menu.draw();
                if (ImGui::Button("Apply")) {
                    reflection_about_the_axis(&storage[0],
                        Axis(axis_menu.selectedItem));
                    drawer.set_vbo("figure", storage);
                }
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Do_Movement();
        Draw(manager, rbr.get_value());
        if (!storage.empty()) {
            tex_drawer.draw(storage[0], camera);
        }
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

bool keys[1024];
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto mat = camera.GetViewMatrix();
    /*for (size_t i = 0; i < mat.length(); i++) {
        for (size_t j = 0; j < mat.length(); j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------" << std::endl;*/
    if (key == GLFW_KEY_F1) {
        camera.Position = glm::vec3(0, 0, 3);
        //camera.Yaw = -40;
        //camera.Pitch = -23.5;
        camera.updateCameraVectors();
    }else if (key == GLFW_KEY_F2) {
        camera.Position = glm::vec3(0, 0, 3);
        //camera.Yaw = -40;
        //camera.Pitch = -23.5;
        camera.updateCameraVectors();
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

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
}

void mouse_scrollback(GLFWwindow* window, double xoffset, double yoffset) {
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
    drawer = Drawer(&mainShader, "vPos", W_WIDTH, W_HEIGHT);
    InitBO(manager);

    glPointSize(5);
    //glViewport(0, 0, W_WIDTH, W_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    manager->checkOpenGLerror();
}

void Draw(OpenGLManager* manager, int mode) {
    glLineWidth(1.0f);
    if (!storage.empty()) {
        drawer.draw(storage, "figure", camera);
    }
    
    //manager->unbind_framebuffer();
    //mainShader.disable_program();
}