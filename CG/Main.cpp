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
using namespace std;

const GLuint W_WIDTH = 1280;
const GLuint W_HEIGHT = 720;
Shader mainShader;
GLint vPos;
GLint funcVPos;
PrimitiveFabric pf;
Drawer drawer;

void Init(OpenGLManager*);
void Draw(OpenGLManager*);
void Release();
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow*, int, int, int);

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
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glewInit();
    auto manager = OpenGLManager::get_instance();
    Init(manager);


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_demo_window = true;
    bool show_another_window = false;
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::vector<string> items = { "Point", "Edge", "Polygon"};
    auto lb = ListBoxWidget<std::string>("Primitive", items);
    auto colorChooser = ColorChooser("Primitive Color");
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            if (colorChooser.draw()) {
                pf.update_color(colorChooser.rgb_value());
            }

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            if (lb.draw()) {
                pf.update_code(lb.selectedItem);
            }
            if (ImGui::Button("Clear")) {
                pf.clear();
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

glm::vec2 axes[] = {
        glm::vec2(-1.0, 0.0),
        glm::vec2(1.0, 0.0),

        glm::vec2(0.0, 1.0),
        glm::vec2(0.0, -1.0)
};
const size_t points_count = 1000 + 1;
glm::vec2 function[points_count];
int left_bound = -3;
int right_bound = 3;
bool refresh = false;
void push_function(float func(float)) {
    float minY = std::numeric_limits<float>::max();
    float maxY = -minY;
    float step = ((float)right_bound - (float)left_bound);
    step /= (float)(points_count - 1);
    float x = (float)left_bound;
    for (size_t i = 0; i < points_count; i++) {
        float y = func(x);
        minY = std::fminf(minY, y);
        maxY = std::fmaxf(maxY, y);
        function[i] = glm::vec2(x, y);
        x += step;
    }
    float x_coef = 2.0f / (right_bound - left_bound);
    float y_coef;
    if (std::abs(maxY - right_bound) < 0.01f && std::abs(minY - left_bound) < 0.01f) {
        y_coef = x_coef;
    }
    else {
        y_coef = 2.0f / (maxY - minY);
    }
    float displace_value;
    if (std::abs(maxY + minY) < 0.01) {
        displace_value = 0;
    }
    else {
        displace_value = (maxY + minY) * 0.5f;
    }
    for (size_t i = 0; i < points_count; i++) {
        float x = (function[i].x) * x_coef;
        float y = (function[i].y - displace_value) * y_coef;
        function[i] = glm::vec2(x, y);
    }

    for (size_t i = 0; i < 2; i++) {
        axes[i].y = 0 - displace_value * x_coef; // т.к. окно квадратное, то для схлопывания в [-1,1] можем использовать x_coef
    }

    auto manager = OpenGLManager::get_instance();
    if (refresh) {
        manager->refresh_vbo("funcVPos", &function, sizeof(function), GL_STATIC_DRAW);
    }
    else {
        manager->init_vbo("funcVPos", &function, sizeof(function), GL_STATIC_DRAW);
        refresh = true;
    }
    manager->refresh_vbo("axesVPos", axes, sizeof(axes), GL_STATIC_DRAW);
    manager->checkOpenGLerror();
}

float sinplus2(float x) {
    return sin(x) + 2;
}
float sinminus2(float x) {
    return sin(x) - 2;
}
float yex(float x) {
    return x;
}
float yenx(float x) {
    return -x;
}
float sqx(float x) {
    return x * x;
}

void key_callback(GLFWwindow* window, int key_code, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key_code == GLFW_KEY_1) {
            push_function(sin);
        }
        else if (key_code == GLFW_KEY_2) {
            push_function(sinplus2);
        }
        else if (key_code == GLFW_KEY_3) {
            push_function(sinminus2);
        }
        else if (key_code == GLFW_KEY_4) {
            push_function(sqx);
        }
        else if (key_code == GLFW_KEY_5) {
            push_function(yex);
        }
        else if (key_code == GLFW_KEY_6) {
            push_function(yenx);
        }
    }
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
            pf.build(xpos, ypos);
            drawer.set_vbo("primitives", pf.get_items());
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            pf.finish_primitive();
        }
    }
    
}


void InitBO(OpenGLManager* manager)
{    
    manager->init_vbo("axesVPos", axes, sizeof(axes), GL_STATIC_DRAW);

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
    pf = PrimitiveFabric(W_WIDTH, W_HEIGHT);
    drawer = Drawer(&mainShader, "vPos");
    vPos = mainShader.get_attrib_location("vPos");
    InitBO(manager);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // белый цвет как цвет заливки
    manager->checkOpenGLerror();
}

void Draw(OpenGLManager* manager) {
    //manager->bind_framebuffer("outputFB");
    glClear(GL_COLOR_BUFFER_BIT);
    //manager->get_texture("outputCB").bind();
    mainShader.use_program();
    mainShader.uniform4f("color", 1.0f, 0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glEnableVertexAttribArray(vPos);
    glBindBuffer(GL_ARRAY_BUFFER, manager->get_buffer_id("axesVPos"));
    glVertexAttribPointer(vPos, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(vPos);
    manager->checkOpenGLerror();
    if (manager->buffer_exists("funcVPos")) {
        mainShader.uniform4f("color", 0.0f, 0.0f, 1.0f, 0.0f);
        glLineWidth(4.0f);
        glEnableVertexAttribArray(vPos);
        glBindBuffer(GL_ARRAY_BUFFER, manager->get_buffer_id("funcVPos"));
        glVertexAttribPointer(vPos, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINE_STRIP, 0, points_count);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(vPos);
    }
    glLineWidth(1.0f);
    if (pf.size() != 0) {
        drawer.draw(pf.get_items(), "primitives");
    }
    //manager->unbind_framebuffer();
    //mainShader.disable_program();
}