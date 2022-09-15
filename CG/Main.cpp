#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "OpenGLWrappers.h"
#include "Primitives.h"
using namespace std;

const GLuint W_WIDTH = 600;
const GLuint W_HEIGHT = 600;
Shader mainShader;
GLint vPos;
GLint funcVPos;

void Init(OpenGLManager*);
void Draw(OpenGLManager*);
void Release();
void key_callback(sf::Keyboard::Key);

int main() {
    sf::Window window(sf::VideoMode(W_WIDTH, W_HEIGHT), "My OpenGL window", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    // Инициализация glew
    glewInit();
    OpenGLManager* manager = OpenGLManager::get_instance();
    Init(manager);

    std::cout << R"(Choose function:
1 - syn(x)
2 - syn(x) + 2
3 - syn(x) - 2
4 - x^2
5 - x
6 - -x)";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::KeyPressed) {
                key_callback(event.key.code);
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        Draw(manager);

        window.display();
    }

    Release();
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
    manager->init_vbo("funcVPos", &function, sizeof(function), GL_STATIC_DRAW);
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

void key_callback(sf::Keyboard::Key key_code)
{
    if (key_code == sf::Keyboard::Num1) {
        push_function(sin);
    }
    else if (key_code == sf::Keyboard::Num2) {
        push_function(sinplus2);
    }
    else if (key_code == sf::Keyboard::Num3) {
        push_function(sinminus2);
    }
    else if (key_code == sf::Keyboard::Num4) {
        push_function(sqx);
    }
    else if (key_code == sf::Keyboard::Num5) {
        push_function(yex);
    }
    else if (key_code == sf::Keyboard::Num6) {
        push_function(yenx);
    }
}


void InitBO(OpenGLManager* manager)
{    
    manager->init_vbo("axesVPos", axes, sizeof(axes), GL_STATIC_DRAW);
    manager->checkOpenGLerror();
}


void Init(OpenGLManager* manager) {
    mainShader = Shader();
    mainShader.init_shader("main.vert", "main.frag");

    vPos = mainShader.get_attrib_location("vPos");
    InitBO(manager);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // белый цвет как цвет заливки
    manager->checkOpenGLerror();
}

void Draw(OpenGLManager* manager) {
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
}