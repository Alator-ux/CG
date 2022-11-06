#pragma once
#include "GL/glew.h"
#include "Texture.h"
#include "Primitives.h"
#include "glm/mat4x4.hpp"
#include "Projection.h"
#include "ThreeDInterface.h"
#include <iostream>
#include "Camera.h"
class TextureDrawer {
    CImgTexture* tex;
    glm::mat4x4 projection;
    float aspect;
    float k = 0.001f;
    void clear() {
        tex->clear();
    }
    HighLevelInterface trim_object(const HighLevelInterface& hlvl_object, Camera& camera) {
        Figure res;
        auto view = camera.GetViewMatrix();
        for (auto& prim : hlvl_object.objects) {
            const Face* face = reinterpret_cast<const Face*>(&prim);
            //glm::vec3 proec = glm::normalize(point_to_2D(face->center(), view));
            glm::vec3 proec = camera.Front;
            if (camera.Front.x + camera.Front.y + camera.Front.z != 0) {
                proec = glm::normalize(camera.Front);
            }
            proec = projection * view * glm::vec4(camera.Front, 1);

            glm::vec3 norm = projection * view * glm::vec4(face->normal, 1);
            float scalar = glm::dot(proec, norm);
            if (scalar > 0) {
                res.objects.push_back(*face);
            }
            else {
                auto a = 0;
            }
        }
        return res;
    }
    glm::vec3 point_to_2D(const glm::vec3& point, const glm::mat4x4& view) {
        auto res = projection * view * glm::vec4(point, 1);
        res *= (1.f / (k * res.z + 1.f));
        res += tex->get_width() / 2;
        return res;
    }
    void draw_polygon_as_2D(const primitives::Primitive& primitive) {
        for (size_t i = 0; i < primitive.points.size() - 1; i++) {
            auto first = primitive.points[i];
            auto second = primitive.points[i + 1];
            //std::cout << "line from x=" << first.x << " y=" << first.y << " to x=" << second.x << " y=" << second.y << std::endl;
            tex->draw_st_line(first, second, primitive.color);
        }

        auto first = primitive.points[0];
        auto second = primitive.points[primitive.points.size() - 1];
        tex->draw_st_line(first, second, primitive.color);
    }
    void draw_polygon(const primitives::Primitive& primitive, const glm::mat4x4& view) {
        for (size_t i = 0; i < primitive.points.size() - 1; i++) {
            auto first = point_to_2D(primitive.points[i], view);
            auto second = point_to_2D(primitive.points[i + 1], view);
            //std::cout << "line from x=" << first.x << " y=" << first.y << " to x=" << second.x << " y=" << second.y << std::endl;
            tex->draw_st_line(first, second, primitive.color);
        }

        auto first = point_to_2D(primitive.points[0], view);
        auto second = point_to_2D(primitive.points[primitive.points.size() - 1], view);
        tex->draw_st_line(first, second, primitive.color);
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
            //projection = projection::perspective(glm::radians(60.0f), aspect, 0.01f, 100.0f);
            projection = projection::perspective(k);
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
        draw_polygon(primitive, view);
    }
    void draw(const HighLevelInterface& highlvl_obj, Camera& camera) {
        clear();
        auto trimed = trim_object(highlvl_obj, camera);
        for (auto& prim : trimed.objects) {
            draw_polygon(prim, camera.GetViewMatrix());
        }
    }
};