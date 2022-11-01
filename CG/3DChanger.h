#pragma once
#include "GL/glew.h"
#include "ThreeDInterface.h"
#include "Primitives.h"

void rotate_line1(primitives::Line* obj, Axis axis, float angle);

void rotate(ThreeDInterface* obj, Axis axis, float angle);

void shift(ThreeDInterface* obj, glm::vec3 vec);

void scale(ThreeDInterface* obj, glm::vec3 vec);

void scale_around_center(ThreeDInterface* obj, glm::vec3 vec);

void rotate_around_line(ThreeDInterface* obj, float angle, glm::vec3 p1, glm::vec3 p2);

void rotate_around_center(ThreeDInterface* obj, Axis axis, float angle);

// Отражение относительно выбранной координатной плоскости
void reflection_about_the_axis(ThreeDInterface* obj, Axis axis);