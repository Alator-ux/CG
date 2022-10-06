#ifndef WIDGETS_H
#define WIDGETS_H
#include "imgui.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Primitives.h"


bool string_item_getter(void* data, int index, const char** output);
bool primitive_item_getter(void* data, int index, const char** output);

class DropDownMenu {
    const char* label;
    std::vector<std::string> items;
public:
    int selectedItem = 0;
    DropDownMenu(const char* label, std::vector<std::string> items) {
        this->label = label;
        this->items = items;
    }
    bool draw() {
        return ImGui::Combo(label, &selectedItem, string_item_getter, 
                            (void*)items.data(), (int)items.size());
    }
};


class ListBox {
    const char* label;
    std::vector<Primitive>* items;
    std::vector<std::string> str_values;
    int height;
public:
    int selectedItem = -1;
    ListBox(const char* label, std::vector<Primitive>* items, int height = 10) {
        this->label = label;
        this->items = items;
        this->height = height;
        for (size_t i = 0; i < this->items->size(); i++) {
            std::stringstream ss;
            ss << "Object " << i;
            this->str_values.push_back(ss.str());
        }
    }
    bool draw() {
        if (str_values.size() + 1 == items->size()) {
            std::stringstream ss;
            ss << "Object " << items->size() - 1;
            this->str_values.push_back(ss.str());
        }
        else if (str_values.size() - 1 == items->size()) {
            str_values.erase(str_values.end() - 1);
            selectedItem -= 1;
        }
        else if (items->size() == 0) {
            if (str_values.size() != 0) {
                str_values.clear();
            }
            selectedItem = -1;
        }
        bool success = ImGui::ListBox(label, &selectedItem, string_item_getter,
                              (void*)str_values.data(), (int)str_values.size(),
                                height);
        return success;
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