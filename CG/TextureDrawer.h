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


    std::vector<float> interpolate(float from_first, float from_second, float to_first, float to_second) {
        std::vector<float> res;

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

    bool out_of_bounds(glm::vec3& p1, glm::vec3& p2) {
        return (p1.x < 0 && p2.x < 0) ||
            (p1.x >= tex->get_width() && p2.x >= tex->get_width()) ||
            (p1.y < 0 && p2.y < 0) ||
            (p1.y >= tex->get_height() && p2.y >= tex->get_height());
    }
    std::vector<glm::vec3> raster_triangle(std::vector<glm::vec3>& triangles, size_t ind) {
        std::vector<glm::vec3> res;
        if (triangles.size() % 3 != 0) {
            return res;
        }
        std::sort(triangles.begin() + ind, triangles.begin() + ind + 3, [](glm::vec3 p1, glm::vec3 p2) {
            return p1.y < p2.y;
            });

        auto xy01 = interpolate(triangles[ind].y, triangles[ind].x, triangles[ind + 1].y, triangles[ind + 1].x);
        auto yz01 = interpolate(triangles[ind].y, triangles[ind].z, triangles[ind + 1].y, triangles[ind + 1].z);

        auto xy02 = interpolate(triangles[ind].y, triangles[ind].x, triangles[ind + 2].y, triangles[ind + 2].x);
        auto yz02 = interpolate(triangles[ind].y, triangles[ind].z, triangles[ind + 2].y, triangles[ind + 2].z);

        auto xy12 = interpolate(triangles[ind + 1].y, triangles[ind + 1].x, triangles[ind + 2].y, triangles[ind + 2].x);
        auto yz12 = interpolate(triangles[ind + 1].y, triangles[ind + 1].z, triangles[ind + 2].y, triangles[ind + 2].z);

        std::vector<float>& xy012 = xy01;
        xy012.pop_back();
        xy012.insert(xy012.end(), xy12.begin(), xy12.end());

        std::vector<float>& yz012 = yz01;
        yz012.pop_back();
        yz012.insert(yz012.end(), yz12.begin(), yz12.end());

        int center = xy012.size() / 2;
        std::vector<float> lx, rx, lz, rz;
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
        int y0 = triangles[ind].y;
        int y2 = triangles[ind + 2].y;

        for (int i = 0; i <= y2 - y0; i++) {
            if (i >= lx.size() || i >= rx.size()) {
                auto a = 1;
            }
            int leftx = lx[i];
            int rightx = rx[i];
            std::vector<float> current_z = interpolate(leftx, lz[i], rightx, rz[i]);
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
            glm::vec3 out;
            bool succ = point_to_2D_bool(point, view, out);
            if (!succ) {
                return std::vector<glm::vec3>();
            }
            res.push_back(out);
        }
        return res;
    }

    std::vector<std::vector<glm::vec3>> raster(HighLevelInterface& highlvl_obj, glm::mat4x4& view) {
        std::vector<std::vector<glm::vec3>> res;

        std::vector<glm::vec3> triangles2D;
        bool one_in_bounds = false;

        for (auto& prim : highlvl_obj.objects) {
            primitives::Polygon* poly = reinterpret_cast<primitives::Polygon*>(&prim);
            std::vector<primitives::Polygon> triangles = triangulate(poly);

            for (auto& triangle : triangles) {
                bool tr_in_bounds = true;
                auto tr2d = triangle_to_2D(triangle, view);
                if (tr2d.size() != 3) {
                    continue;
                }
                for (size_t i = 0; i < 3; i++) {
                    tr_in_bounds = tr_in_bounds &&
                        !out_of_bounds(tr2d[i], tr2d[(i + 1) % 3]);
                }
                one_in_bounds = one_in_bounds || tr_in_bounds;
                triangles2D.insert(triangles2D.end(), tr2d.begin(), tr2d.end());
            }
        }
        if (!one_in_bounds) {
            return res;
        }
       for (size_t i = 0; i < triangles2D.size(); i += 3) {
           auto rastered = raster_triangle(triangles2D, i);
           res.push_back(rastered);
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
            glm::vec3 norm = calc_normal(face);
            float scalar = glm::dot(proec, norm);
            if (scalar < 0) {
                res.objects.push_back(*face);
            }
        }
        return res;
    }
    glm::vec3 point_to_2D(const glm::vec3& point, const glm::mat4x4& view) {
        auto res = glm::vec4(point, 1);
        /*res = view * projection * res;
        res *= (1.f / (k * -res.z + 1.f));
        res.x += tex->get_width() / 2;
        res.y += tex->get_height() / 2;*/
        auto center = tex->get_width() / 2;
        res = projection * view * res;
        if (std::abs(res.w) == 0) {
            return glm::vec3(-1, -1, -1);
        }
        res *= 1.0f / res.w;
        res.x = center + center * res.x;
        res.y = center + center * res.y;

        return res;
    }
    bool point_to_2D_bool(const glm::vec3& point, glm::mat4x4& view, glm::vec3& out) {
        auto res = glm::vec4(point, 1);
        /*res = view * projection * res;
        res *= (1.f / (k * -res.z + 1.f));
        res.x += tex->get_width() / 2;
        res.y += tex->get_height() / 2;*/
        auto center = tex->get_width() / 2;
        res = projection * view * res;
        if (std::abs(res.w) == 0) {
            return false;
        }
        if (res.z < 0) {
            return false;
        }
        res *= 1.0f / res.w;
        res.x = center + center * res.x;
        res.y = center + center * res.y;
        out = res;
        return true;
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
    void draw_trimed_primitive(primitives::Primitive& primitive, Camera& camera) {
        auto view = camera.GetViewMatrix();
        auto out = primitives::Polygon();
        out.color = primitive.color;
        for (size_t i = 0; i < primitive.points.size(); i++) {
            glm::vec3 p;
            auto succ = point_to_2D_bool(primitive.points[i], view, p);
            if (succ) {
                out.push_point(p);
            }
            //std::cout << "line from x=" << first.x << " y=" << first.y << " to x=" << second.x << " y=" << second.y << std::endl;
        }
        if (out.points.size() < 3) {
            return;
        }
        glm::vec3 proec = camera.Front;
        glm::vec3 norm = calc_normal(&out);
        float scalar = glm::dot(proec, norm);
        if (scalar < 0) {
            draw_polygon_as_2D(out);
        }
    }
    void draw_polygon(primitives::Primitive& primitive, Camera& camera) {
        auto view = camera.GetViewMatrix();
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
            projection = projection::perspective(glm::radians(45.0f), aspect, 0.001f, 1000.0f);
            //projection = projection::perspective(k);
            //projection = projection::perspective3(45.0f, tex->get_width(), tex->get_height(), 0.001f, 1000.0f);
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
        //draw_polygon(primitive, view);
    }
    void draw(HighLevelInterface& highlvl_obj, Camera& camera) {
        clear();
        /*for (auto& prim : highlvl_obj.objects) {
            draw_trimed_primitive(prim, camera);
        }*/

        std::vector<HighLevelInterface> vect;
        vect.push_back(highlvl_obj);
        z_buffer(vect, camera);
    }
};