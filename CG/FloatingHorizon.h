#pragma once
#include "GL/glew.h"
#include "Primitives.h"
#include <vector>
#include <functional>
#include "Texture.h"
#include "Primitives.h"
#include "glm/mat4x4.hpp"
#include <iostream>
#include "Camera.h"
#include <limits>
class FloatingHorizon {
    CImgTexture* tex;
    std::vector<float> ups;
    std::vector<float> downs;
    float bound = 5.f;
    float gap = 75.f;
    float scale_coef = 100.f;

    glm::mat4x4 projection;
    float aspect;
    float k = 0.001f; 

    void clear() {
        tex->clear();
        ups = std::vector<float>(tex->get_width(), -std::numeric_limits<float>::max());
        downs = std::vector<float>(tex->get_width(), std::numeric_limits<float>::max());
    }
    bool point_to_2D_bool(const glm::vec3& point, glm::mat4x4& view, glm::vec3& out) {
        auto res = glm::vec4(point, 1);
        auto center = tex->get_width() / 2;
        res = projection * view * res;
        if (std::abs(res.w) == 0) {
            return false;
        }
        if (res.z < 0) {
            //return false;
        }
        res.x *= 1.0f / res.w;
        res.y *= 1.0f / res.w;
        res.x = center + center * res.x;
        res.y = center + center * res.y;
        out = res;
        return true;
    }

    glm::vec3 transform(glm::vec3 point, float yaw, float pitch);
    primitives::HidingPoint try_to_hide(glm::vec3 p) {
        auto hp = primitives::HidingPoint(p);
        int x = hp.points[0].x;
        int y = hp.points[0].y;
        if (x < 0 || x >= tex->get_width()) {
            hp.vmode = primitives::HidingPoint::Visibility::invisible;
            return hp;
        }
        if (y >= ups[x]) {
            hp.vmode = primitives::HidingPoint::Visibility::up;
            return hp;
        }
        if (y < ups[x]) {
            auto a = 1;
        }
        if (y <= downs[x]) {
            hp.vmode = primitives::HidingPoint::Visibility::down;
            return hp;
        }

        hp.vmode = primitives::HidingPoint::Visibility::invisible;
        return hp;
    }
    primitives::HidingPoint get_hiding_point(glm::vec3 p, Camera& camera) {
        glm::vec3 transformed;
        //transformed = transform(p, camera.Yaw, camera.Pitch);
        transformed = transform(p, 0, 0);
        
        auto hp = try_to_hide(transformed);
        return hp;
    }
    void update_vecs(primitives::HidingPoint prev, primitives::HidingPoint cur) {
        if (cur.points[0].x < 0 || cur.points[0].x >= tex->get_width()) {
            return;
        }
        if (prev.points[0].x < 0) {
            return;
            /*auto tg = (cur.points[0].y) / (cur.points[0].x);
            for (int x = cur.points[0].x; x >= 0; x--) {
                auto y = tg * (x) + [0].y;
                ups[x] = std::max(ups[x], y);
                downs[x] = std::min(downs[x], y);
            }*/
        }
        else if ((int)prev.points[0].x == (int)cur.points[0].x) {
            ups[cur.points[0].x] = std::max(ups[cur.points[0].x], cur.points[0].y);
            downs[cur.points[0].x] = std::min(downs[cur.points[0].x], cur.points[0].y);
        }
        else {
            auto tg = (cur.points[0].y - prev.points[0].y) / (cur.points[0].x - prev.points[0].x);
            for (int x = prev.points[0].x; x <= cur.points[0].x; x++) {
                auto y = tg * (x - prev.points[0].x) + prev.points[0].y;
                ups[x] = std::max(ups[x], y);
                downs[x] = std::min(downs[x], y);
            }
        }
        /*if (prev.vmode == primitives::HidingPoint::Visibility::up) {
            std::cout << "x=" << prev.points[0].x << "; y=" << prev.points[0].y << 
                "ups[x]=" << ups[prev.points[0].x] << "\n";
        }*/
    }
    /// <summary>
    /// 
    /// </summary>
    /// <param name="p1"></param>
    /// <param name="p2"></param>
    /// <param name="camera"></param>
    /// <param name="reverse"></param>
    /// <returns></returns>
    primitives::HidingPoint find_intersect(primitives::HidingPoint p1, primitives::HidingPoint p2,
        Camera& camera, bool reverse = false) {
        float xStep = (p2.points[0].x - p1.points[0].x * 1.0) / 20;
        float yStep = (p2.points[0].y - p1.points[0].y * 1.0) / 20;
        for (int i = 1; i <= 20; i++) {
            auto p = try_to_hide(glm::vec3(
                glm::clamp(p1.points[0].x + i * xStep, 0.f, (float)(tex->get_width() - 1)),
                p1.points[0].y + i * yStep,
                p2.points[0].z
            ));
            if (reverse && p.vmode != primitives::HidingPoint::Visibility::invisible) {
                return p;
            }
            else if (p.vmode == primitives::HidingPoint::Visibility::invisible) {
                return p;
            }
        }

        return p2;
    }
    glm::vec2 to_2D(primitives::HidingPoint hp) {
        return glm::vec2(
            glm::clamp(hp.points[0].x, 0.f, (float)(tex->get_width() - 1)),
            glm::clamp((float)tex->get_height() / 2.f - hp.points[0].y, 0.f, (float)(tex->get_height() - 1))
        );
    }
    void draw(primitives::HidingPoint hp1, primitives::HidingPoint hp2, glm::vec3 color) {
        tex->draw_st_line(to_2D(hp1), to_2D(hp2), color);
    }
public:
    FloatingHorizon(CImgTexture* tex) {
        this->tex = tex;
        this->projection = glm::mat4x4(1);
        this->aspect = (float)tex->get_width() / (float)tex->get_height();
    }
    
    void draw(std::function<float(float, float)> f, Camera& camera) {
        clear();

        auto view = camera.GetViewMatrix();

        float step = bound * 2.f / gap;

        for (float z = bound; z >= -bound; z -= step) {
            auto prev_hp = get_hiding_point(glm::vec3(-bound, f(-bound, z), z), camera);

            for (float x = -bound; x <= bound; x += step) {
                auto cur_hp = get_hiding_point(glm::vec3(x, f(x, z), z), camera);

                if (cur_hp.vmode == primitives::HidingPoint::Visibility::up) {
                    if (prev_hp.vmode == primitives::HidingPoint::Visibility::invisible) {
                        auto inter = find_intersect(cur_hp, prev_hp, camera);
                        // draw from inter to cur
                        draw(cur_hp, inter, glm::vec3(0));
                        update_vecs(inter, cur_hp);
                    }
                    else {
                        // draw from prev to cur
                        draw(prev_hp, cur_hp, glm::vec3(0));
                        update_vecs(prev_hp, cur_hp);
                    }
                }
                else if (cur_hp.vmode == primitives::HidingPoint::Visibility::down) {
                    if (prev_hp.vmode == primitives::HidingPoint::Visibility::invisible) {
                        auto inter = find_intersect(cur_hp, prev_hp, camera);
                        // draw fro, inter to cur
                        draw(cur_hp, inter, glm::vec3(125));
                        update_vecs(inter, cur_hp);
                    }
                    else {
                        // draw from prev to cur
                        draw(prev_hp, cur_hp, glm::vec3(125));
                        update_vecs(prev_hp, cur_hp);
                    }
                }
                else {
                    if (prev_hp.vmode == primitives::HidingPoint::Visibility::up) {
                        auto inter = find_intersect(cur_hp, prev_hp, camera, true);
                        // draw from prev to inter
                        draw(prev_hp, inter, glm::vec3(0));
                        update_vecs(prev_hp, inter);
                    }
                    else if (prev_hp.vmode == primitives::HidingPoint::Visibility::down) {
                        auto inter = find_intersect(cur_hp, prev_hp, camera, true);
                        // draw from prev to inter
                        draw(prev_hp, inter, glm::vec3(125));
                        update_vecs(prev_hp, inter);
                    }
                    else {
                        auto a = 1;
                    }
                }
                prev_hp = cur_hp;
            }
        }
    }
};