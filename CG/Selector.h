#pragma once
#include "Primitives.h"
#include <glm/fwd.hpp>
#include <vector>

class Selector {
    std::vector<Primitive>* storage;
    int current_item;
public:
    Selector() {
        this->storage = nullptr;
        this->current_item = -1;
    }
    Selector(std::vector<Primitive>* storage) {
        this->storage = storage;
        this->current_item = -1;
    }
    void set_item_index(int index) {
        this->current_item = index;
    }
    Primitive* get_item() {
        if (current_item < 0) {
            return nullptr;
        }
        return &(*storage)[current_item];
    }
};