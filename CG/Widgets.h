#ifndef WIDGETS_H
#define WIDGETS_H
#include "imgui.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

bool item_getter(void* data, int index, const char** output);

template <typename T>
class ListBoxWidget {
    const char* label;
    std::vector<T> items;
public:
    int selectedItem = 0;
    ListBoxWidget(const char* label, std::vector<T> items) {
        this->label = label;
        this->items = items;
    }
    bool draw() {
        return ImGui::ListBox(label, &selectedItem, item_getter, (void*)items.data(), (int)items.size());
    }
};

class ColorChooser {
    const char* label;
    glm::vec4 color;
public:
    ColorChooser(const char* label) {
        this->label = label;
        this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    bool draw() {
        return ImGui::ColorEdit3(label, (float*)&color);
    }
    glm::vec3 rgb_value() {
        return glm::vec3(color);
    }
};


#endif