#ifndef WIDGETS_H
#define WIDGETS_H
#include "imgui.h"
#include <vector>
#include <string>

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


#endif