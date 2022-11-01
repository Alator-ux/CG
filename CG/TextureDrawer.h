#pragma once
#include "Texture.h"
#include "Primitives.h"
#include "glm/mat4x4.hpp"
class TextureDrawer {
    CImgTexture* tex;
    glm::mat4x4 projection;
    float aspect;
    void clear() {
        tex->clear();
    }
public:
    TextureDrawer(CImgTexture* tex) {
        this->tex = tex;
        this->projection = glm::mat4x4(1);
        this->aspect = (float)tex->get_width() / (float)tex->get_height();
    }
    void set_projection_mode(projection::Type type) {
        switch (type)
        {
        case projection::pers:
        {
            projection = projection::perspective(glm::radians(60.0f), aspect, 0.001f, 500.0f);
            break;
        }
        case projection::axon:
        {
            projection = projection::axonometric(PI / 6, -(PI / 3));
            break;
        }
        case projection::none:
        {
            projection = glm::mat4(1);
            break;
        }
        default:
            break;
        }
    }
    void draw(primitives::Primitive primitive, const glm::mat4x4& view) {
        clear();
        for (size_t i = 0; i < primitive.points.size() - 1; i++) {
            auto first = projection * view * glm::vec4(primitive.points[i], 1);
            //first /= first.w;
            auto second = projection * view * glm::vec4(primitive.points[i + 1], 1);
           // second /= second.w;
            tex->draw_line(first, second, primitive.color);
        }
        auto first = projection * view * glm::vec4(primitive.points[0], 1);
       // first /= first.w;
        auto second = projection * view * glm::vec4(primitive.points[primitive.points.size() - 1], 1);
       // second /= second.w;
        tex->draw_line(first, second, primitive.color);
    }
};