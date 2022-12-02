#ifndef WIDGETS_H
#define WIDGETS_H
#include "imgui.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Primitives.h"
#include <sstream>

bool string_item_getter(void* data, int index, const char** output);
bool primitive_item_getter(void* data, int index, const char** output);

class DropDownMenu {
    const char* label;
    std::vector<std::string> items;
    float width;
public:
    int selectedItem = 0;
    DropDownMenu(const char* label, std::vector<std::string> items, float width = -1) {
        this->label = label;
        this->items = items;
        this->width = width;
    }
    bool draw() {
        if (width == -1) {
            return ImGui::Combo(label, &selectedItem, string_item_getter,
                (void*)items.data(), (int)items.size());
        }
        ImGui::PushItemWidth(width);
        bool touched = ImGui::Combo(label, &selectedItem, string_item_getter,
            (void*)items.data(), (int)items.size());
        ImGui::PopItemWidth();
        return touched;
    }
};


class ListBox {
    const char* label;
    std::vector<primitives::Primitive>* items;
    std::vector<std::string> str_values;
    int height;
public:
    int selectedItem = -1;
    ListBox(const char* label, std::vector<primitives::Primitive>* items, int height = 10) {
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

class IntSlider {
    const char* label;
    int value;
    int min_value;
    int max_value;
public:
    IntSlider(const char * label, int min_value, int max_value) {
        this->label = label;
        this->value = min_value;
        this->min_value = min_value;
        this->max_value = max_value;
    }
    bool draw() {
        return ImGui::SliderInt(label, &value, min_value, max_value);
    }
    int get_value() {
        return value;
    }
};

class FloatSlider {
    const char* label;
    float value;
    float min_value;
    float max_value;
public:
    FloatSlider(const char* label, float min_value, float max_value) {
        this->label = label;
        this->value = min_value;
        this->min_value = min_value;
        this->max_value = max_value;
    }
    bool draw() {
        return ImGui::SliderFloat(label, &value, min_value, max_value);
    }
    float get_value() {
        return value;
    }
};

class RadioButton {
    const char* label;
    bool activated;
public:
    RadioButton(const char* label) {
        this->label = label;
        this->activated = false;
    }
    RadioButton(const char* label, bool activated) {
        this->label = label;
        this->activated = activated;
    }
    bool draw() {
        return ImGui::RadioButton(label, activated);
    }
    void activate(){
        this->activated = true;
    }
    void disable() {
        this->activated = false;
    }
};

class RadioButtonRow {
    std::vector<RadioButton> buttons;
    std::vector<std::string> labels;
    int active = 0;
public:
    RadioButtonRow(std::vector<std::string> labels) {
        this->labels = labels;
        bool flag = true;
        for (std::string& label : this->labels) {
            auto r = RadioButton(label.c_str(), flag);
            buttons.push_back(r);
            flag = false;
        }
    }
    bool draw() {
        bool was_pressed = false;
        for (size_t i = 0; i < buttons.size(); i++) {
            bool pressed = buttons[i].draw();
            if (i != buttons.size() - 1) {
                ImGui::SameLine();
            }
            if (pressed && active != i) {
                buttons[i].activate();
                buttons[active].disable();
                active = i;
                was_pressed = true;
            }
        }

        return was_pressed;
    }
    int get_value() {
        return active;
    }
    std::string get_label() {
        return active < 0 ? "" : labels[active];
    }
};

class InputFloat {
    std::string label;
    float value = 0.0f;
public:
    InputFloat(const std::string& label) {
        this->label = label;
    }
    bool draw() {
        ImGui::PushItemWidth(60);
        bool touched = ImGui::InputFloat(label.c_str(), &value);
        ImGui::PopItemWidth();
        return touched;
    }
    float get_value() {
        return value;
    }
};

class Vec3Selector {
    std::vector<InputFloat> text_fields;
    static char identifier;
public:
    Vec3Selector() {
        char id[1];
        id[0] = identifier;
        std::string label = "x##";
        label.append(id);
        text_fields.push_back(InputFloat(label));
        label = "y##";
        label.append(id);
        text_fields.push_back(InputFloat(label));
        label = "z##";
        label.append(id);
        text_fields.push_back(InputFloat(label));
        identifier++;
    }
    bool draw() {
        bool touched = false;
        for (size_t i = 0; i < text_fields.size(); i++) {
            touched = touched || text_fields[i].draw();
            if (i != text_fields.size() - 1) {
                ImGui::SameLine();
            }
        }
        return touched;
    }
    glm::vec3 get_value() {
        auto x = text_fields[0].get_value();
        auto y = text_fields[1].get_value();
        auto z = text_fields[2].get_value();
        return glm::vec3(x, y, z);
    }
};

#endif