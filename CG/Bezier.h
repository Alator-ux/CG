#pragma once
#include "GL/glew.h"
#include "Primitives.h"
#include <vector>

class Bezier {
    std::vector<BezierPoint> bez_points;
    Polygon curve;
    int active_p;
    glm::vec3 point_color;
    void generate_part(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) {
        curve.push_point(p1);
        glm::vec3 last = p1;
        for (float t = 0.05; t <= 1; t += 0.05) {
            int x = std::pow(1 - t, 3) * p1.x + 3 * std::pow(1 - t, 2) * t * p2.x +
                3 * (1 - t) * std::pow(t, 2) * p3.x + std::pow(t, 3) * p4.x;
            int y = std::pow(1 - t, 3) * p1.y + 3 * std::pow(1 - t, 2) * t * p2.y +
                3 * (1 - t) * std::pow(t, 2) * p3.y + std::pow(t, 3) * p4.y;
            last = glm::vec3(x, y, 1);
            curve.push_point(last);
        }
        curve.push_point(p4);
    }
public:
    Bezier() {
        this->bez_points = std::vector<BezierPoint>();
        this->point_color = glm::vec3(1);
        this->curve = Polygon(glm::vec3(-1), glm::vec3(1));
        this->curve.points.clear();
        this->active_p = -1;
    }
    void generate() {
        auto size = bez_points.size();
        if (size < 2) {
            return;
        }
        curve.points.clear();
        if (size == 2) {
            curve.push_point(bez_points[0].get_main());
            curve.push_point(bez_points[1].get_main());
        }
        if (size < 4){
            //return;
        }
        for (auto i = 0; i < size - 3; i+=3) {
            auto p0 = bez_points[i].get_main();
            auto p1 = bez_points[i+1].get_main();
            auto p2 = bez_points[i+2].get_main();
            auto p3 = bez_points[i+3].get_main();
            generate_part(p0, p1, p2, p3);
        }
        /*
        
        auto p0 = bez_points[0].get_main();
        auto p1 = bez_points[1].get_main();
        auto p2 = bez_points[2].get_main();
        auto mid = glm::vec3((p1.x + p2.x) / 2, (p1.y + p2.y) / 2, 1);
        generate_part(p0, p1, mid, p2);
        for (auto i = 3; i < size - 1; i++) {
            p0 = mid;
            p1 = bez_points[i].get_main();
            p2 = bez_points[i+1].get_main();
            mid = glm::vec3((p1.x + p2.x) / 2, (p1.y + p2.y) / 2, 1);
            generate_part(p0, p1, mid, p2);
        }
        */
        /*
        for (size_t i = 0; i < bez_points.size() - 2; i += 2) {
            glm::vec3& bp1 = bez_points[i].get_main();
            glm::vec3& bp2 = bez_points[i+1].get_main();
            glm::vec3& bp3 = bez_points[i + 2].get_main();
            auto mid = glm::vec3((bp2.x + bp3.x) / 2, (bp2.y + bp3.y) / 2, 1);
            generate_part(bp1, bp2, mid, bp3);
        }
        */
    }
    void add_point(glm::vec3 coord) {
        curve.points.clear();
        bez_points.push_back(BezierPoint(coord, point_color));
    }
    void delete_point() {
        if (active_p < 0|| active_p >= bez_points.size()) {
            return;
        }
        curve.points.clear();
        bez_points.erase(bez_points.begin() + active_p);
    }
    void shift_point(glm::vec3 point_to) {
        if (active_p < 0 || active_p >= bez_points.size()) {
            return;
        }
        curve.points.clear();
        Primitive* prim = reinterpret_cast<Primitive*>(&bez_points[active_p]);
        PrimitiveChanger().shift(prim, point_to);
    }
    void set_active_point(size_t ind) {
        active_p = ind;
    }
    void set_line_color(glm::vec3 color) {
        curve.color = color;
    }
    void set_points_color(glm::vec3 color) {
        point_color = color;
    }
    void clear() {
        curve.points.clear();
        bez_points.clear();
    }
    Primitive* get_line() {
        if (curve.points.size() == 0) {
            return nullptr;
        }
        auto prim = reinterpret_cast<Primitive*>(&curve);
        return prim;
    }
    std::vector<Primitive> get_points() {
        auto res = std::vector<Primitive>();
        for (BezierPoint& bp : bez_points) {
            Point p = Point(bp.get_main(), point_color);
            Primitive* prim = reinterpret_cast<Primitive*>(&p);
            res.push_back(*prim);
        }
        return res;
    }
    std::vector<Primitive>* get_bezier_points() {
        auto res = reinterpret_cast<std::vector<Primitive>*>(&bez_points);
        return res;
    }
};
