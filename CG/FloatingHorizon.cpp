#include "FloatingHorizon.h"
#include "glm/glm.hpp"
#include "3DChanger.h";

glm::vec3 FloatingHorizon::transform(glm::vec3 point, float yaw, float pitch) {
    auto res = rotate(point, Axis::oy, yaw);
    res = rotate(res, Axis::ox, pitch);
    res.x = tex->get_width() / 2 + scale_coef * res.x;
    res.y = scale_coef * res.y;
    res.z = scale_coef * res.z;
    return res;
}