#include "Widgets.h"

bool item_getter(void* data, int index, const char** output)
{
    std::string* items = (std::string*)data;
    std::string& current_item = items[index];

    *output = current_item.c_str();

    return true;
}