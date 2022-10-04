#pragma once
#include "Primitives.h"
#include <vector>

class PrimitiveChanger {
    std::vector<Primitive>* storage;
    int active_item;
    int mode;
public:
    PrimitiveChanger(std::vector<Primitive>* storage) {
        this->storage = storage;
        this->active_item = -1;
    }
};