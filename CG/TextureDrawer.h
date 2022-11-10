#pragma once
#include "GL/glew.h"
#include "Texture.h"
#include "Primitives.h"
#include "glm/mat4x4.hpp"
#include "Projection.h"
#include "ThreeDInterface.h"
#include <iostream>
#include "Camera.h"
#include <limits>
class TextureDrawer {
    CImgTexture* tex;
    glm::mat4x4 projection;
    float aspect;
    float k = 0.001f;


    std::vector<int> interpolate(float from_first, float from_second, float to_first, float to_second) {
        std::vector<int> res;

        auto denominator = std::abs((int)to_first - (int)from_first);
        if (denominator < 1e-3) {
            res.push_back(from_second);
            return res;
        }

        float step = (to_second - from_second) / (to_first - from_first);

        float y = from_second;
        for (int i = from_first; i <= to_first; i++) {
            res.push_back(y);
            y += step;
        }

        return res;
    }

    std::vector<glm::vec3> raster_triangle(std::vector<glm::vec3> triangle) {
        std::vector<glm::vec3> res;
        std::sort(triangle.begin(), triangle.end(), [](glm::vec3 p1, glm::vec3 p2) {
            return p1.y < p2.y;
            });

        auto xy01 = interpolate(triangle[0].y, triangle[0].x, triangle[1].y, triangle[1].x);
        auto yz01 = interpolate(triangle[0].y, triangle[0].z, triangle[1].y, triangle[1].z);

        auto xy02 = interpolate(triangle[0].y, triangle[0].x, triangle[2].y, triangle[2].x);
        auto yz02 = interpolate(triangle[0].y, triangle[0].z, triangle[2].y, triangle[2].z);

        auto xy12 = interpolate(triangle[1].y, triangle[1].x, triangle[2].y, triangle[2].x);
        auto yz12 = interpolate(triangle[1].y, triangle[1].z, triangle[2].y, triangle[2].z);

        std::vector<int>& xy012 = xy01;
        xy012.pop_back();
        xy012.insert(xy012.end(), xy12.begin(), xy12.end());

        std::vector<int>& yz012 = yz01;
        yz012.pop_back();
        yz012.insert(yz012.end(), yz12.begin(), yz12.end());

        int center = xy012.size() / 2;
        std::vector<int> lx, rx, lz, rz;
        if (xy02[center] < xy012[center]) {
            lx = xy02;
            lz = yz02;
            rx = xy012;
            rz = yz012;
        }
        else {
            lx = xy012;
            lz = yz012;
            rx = xy02;
            rz = yz02;
        }
        int y0 = triangle[0].y;
        int y2 = triangle[2].y;

        for (int i = 0; i <= y2 - y0; i++) {
            if (i >= lx.size() || i >= rx.size()) {
                auto a = 1;
            }
            int leftx = lx[i];
            int rightx = rx[i];
            std::vector<int> current_z = interpolate(leftx, lz[i], rightx, rz[i]);
            for (int j = leftx; j <= rightx; j++) {
                if (j - leftx < 0 || j - leftx >= current_z.size()) {
                    auto a = 1;
                }
                res.push_back(glm::vec3(j, y0 + i, current_z[j - leftx]));
            }
        }
        return res;
    }

    std::vector<primitives::Polygon> triangulate(primitives::Polygon* poly) {
        std::vector<primitives::Polygon> res;
        if (poly->points.size() == 3) {
            res.push_back(*poly);
            return res;
        }

        for (size_t i = 2; i < poly->points.size(); i++) {
            auto triangle = primitives::Polygon(poly->points[0], glm::vec3(1));
            triangle.push_point(poly->points[i - 1]);
            triangle.push_point(poly->points[i]);
            res.push_back(triangle);
        }

        return res;
    }

    std::vector<glm::vec3> triangle_to_2D(primitives::Primitive triangle, glm::mat4x4& view) {
        std::vector<glm::vec3> res;
        for (auto& point : triangle.points) {
            res.push_back(point_to_2D(point, view));
        }
        return res;
    }

    std::vector<std::vector<glm::vec3>> raster(HighLevelInterface& highlvl_obj, glm::mat4x4& view) {
        std::vector<std::vector<glm::vec3>> res;
        for (auto& prim : highlvl_obj.objects) {
            primitives::Polygon* poly = reinterpret_cast<primitives::Polygon*>(&prim);
            std::vector<primitives::Polygon> triangles = triangulate(poly);

            std::vector<glm::vec3> face;
            for (auto& triangle : triangles) {
                auto rastered = raster_triangle(triangle_to_2D(triangle, view));
                face.insert(face.end(), rastered.begin(), rastered.end());
            }
            res.push_back(face);
        }
        return res;
    }

    bool in_bounds(int x, int y) {
        return -1 < x && x < tex->get_width() && -1 < y && y < tex->get_height();
    }

    void z_buffer(std::vector<HighLevelInterface>& highlvl_objs, Camera& camera) {
        std::vector<std::vector<float>> buffer(
            tex->get_height(), std::vector<float>(tex->get_width(), std::numeric_limits<float>::max())
        );
        glm::mat4x4 view = camera.GetViewMatrix();
        std::vector<std::vector<std::vector<glm::vec3>>> rastered_objs;
        for (auto& obj : highlvl_objs) {
            rastered_objs.push_back(raster(obj, view));
        }

        size_t obj_ind = 0;
        for (auto& rastered_obj : rastered_objs) {
            size_t prim_ind = 0;
            for (auto& rastered_prim : rastered_obj) {
                for (auto& point : rastered_prim) {
                    int x = point.x;
                    int y = point.y;
                    if (x ==0 || x == -1) {
                        auto a = 1;
                    }
                    //std::cout << "x=" << x << "; y=" << y << std::endl;
                    if (in_bounds(x, y)) {
                        if (point.z < buffer[x][y]) {
                            buffer[x][y] = point.z;
                            tex->set_rgb(x, y, highlvl_objs[obj_ind].objects[prim_ind].color);
                        }
                    }
                }
                prim_ind++;
            }
            obj_ind++;
        }
    }
    void clear() {
        tex->clear();
    }
    glm::vec3 calc_normal(primitives::Polygon* face) {
        auto v1 = face->points[1] - face->points[0];
        auto v2 = face->points[face->points.size() - 1] - face->points[0];
        return glm::normalize(glm::cross(v1, v2));
    }
    HighLevelInterface trim_object(HighLevelInterface& hlvl_object, Camera& camera) {
        Figure res;
        auto view = camera.GetViewMatrix();
        for (auto& prim : hlvl_object.objects) {
            primitives::Polygon* face = reinterpret_cast<primitives::Polygon*>(&prim);
            glm::vec3 proec = camera.Front;
            glm::vec3 norm = norm = calc_normal(face);
            float scalar = glm::dot(proec, norm);
            if (scalar < 0) {
                res.objects.push_back(*face);
            }
        }
        return res;
    }
    glm::vec3 point_to_2D(const glm::vec3& point, const glm::mat4x4& view) {
        auto res = glm::vec4(point, 1);
        res = projection * view * res;
        res *= (1.f / (k * -res.z + 1.f));
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
        glm::vec3(TextureDrawer:: * _point_to_2D)(const glm::vec3 & point, const glm::mat4x4 & view);
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
    void draw(HighLevelInterface& highlvl_obj, Camera& camera) {
        clear();
        //auto trimed = trim_object(highlvl_obj, camera);
        /*for (auto& prim : highlvl_obj.objects) {
            draw_polygon(prim, camera.GetViewMatrix());
        }*/
        std::vector<HighLevelInterface> vect;
        vect.push_back(highlvl_obj);
        z_buffer(vect, camera);
    }
};