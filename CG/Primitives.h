#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <functional>
#include <vector>
struct Color {
    glm::vec4 rgba;
    Color() {
        rgba = glm::vec4(0.0f);
    }
    Color(glm::vec4 rgba) {
        this->rgba = rgba;
    }
    Color(glm::vec3 rgb) {
        this->rgba = glm::vec4(rgb, 1.0f);
    }
    glm::vec3 to_vec3() {
        return glm::vec3(rgba.r, rgba.g, rgba.b);
    }
    static Color red() {
        return Color(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    static Color green() {
        return Color(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    static Color blue() {
        return Color(glm::vec3(0.0f, 0.0f, 1.0f));
    }
    static Color white() {
        return Color(glm::vec3(1.0f, 1.0f, 1.0f));
    }
    static Color copper() {
        return Color(glm::vec3(1.0f, 0.5f, 0.31f));
    }
    static Color dark_olive_green() {
        return Color(glm::vec3(0.33f, 0.42f, 0.18f));
    }
};


const double PI = 3.141592653589793238463;

inline double toRadians(double angle) {
    return PI * angle / 180.0;
}

enum Axis{ox, oy, oz};

namespace primitives {
    enum Type {
        base, point, bezpoint, edge, line, segment, polygon
    };
    struct Primitive {
    public:
        std::vector<glm::vec3> points;
        std::vector<glm::vec3> colors;
        std::vector<glm::vec2> uv_vec;
        int drawing_type;
        float width = 1;
        Type type = base;
        GLint get_points_count() {
            return (GLint)points.size();
        }
        void push_point(glm::vec2 coords) {

        }

        bool primitive_is_finished() {
            return true;
        }
        void set_uv_vec(std::vector<glm::vec2> uv_vec) {
            this->uv_vec = uv_vec;
        }
        static std::string get_string_name() {
            return "Primitive";
        }
    };

    struct Point : public Primitive {
    protected:
        Point(){}
    public:
        Point(glm::vec3 coord, glm::vec2 uv) {
            points.push_back(coord);
            uv_vec.push_back(uv);
            drawing_type = GL_POINTS;
            Type type = point;
        }
        Point(glm::vec3 coord, glm::vec3 color) {
            points.push_back(coord);
            colors.push_back(color);
            drawing_type = GL_POINTS;
            Type type = point;
        }
        static std::string get_string_name() {
            return "Point";
        }
    };

    struct HidingPoint : public primitives::Point {
    public:
        enum class Visibility {
            up, down, invisible
        };
        Visibility vmode;
        HidingPoint(glm::vec3 coord) {
            this->points.push_back(coord);
        }
        HidingPoint(glm::vec3 coord, Visibility vmode) {
            this->points.push_back(coord);
            this->vmode = vmode;
        }
    };

    struct BezierPoint : public Primitive {
        BezierPoint(glm::vec3 coord, glm::vec3 color, int offset = 50) {
            this->points.push_back(coord);
            auto help_p = glm::vec3(coord.x - offset, coord.y, 1);
            this->points.push_back(help_p);
            help_p = glm::vec3(coord.x + offset, coord.y, 1);
            this->points.push_back(help_p);

            this->drawing_type = GL_POINTS;
            colors.push_back(color);
            Type type = bezpoint;
        }
    };

    struct Edge : public Primitive {
    public:
        Edge(glm::vec3 coords, glm::vec3 color) {
            points.push_back(coords);
            colors.push_back(color);
            drawing_type = GL_POINTS;
            Type type = edge;
        }
        Edge(glm::vec3 first, glm::vec3 second, glm::vec3 color, float width = 1) {
            points.push_back(first);
            push_point(second);
            colors.push_back(color);
            this->width = width;
        }
        void push_point(glm::vec3 coords) {
            if (points.size() == 1) {
                points.push_back(coords);
                drawing_type = GL_LINES;
            }
        }
        bool primitive_is_finished() {
            return points.size() == 2;
        }
        static std::string get_string_name() {
            return "Edge";
        }
    };

    struct Line : public Primitive {
        Line() {}
        Line(glm::vec3 coords, glm::vec3 color) {
            points.push_back(coords);
            colors.push_back(color);
            drawing_type = GL_POINTS;
            Type type = line;
        }
        void push_point(glm::vec3 coords) {
            points.push_back(coords);
            drawing_type = GL_LINE_STRIP;
        }
        void push_point(glm::vec3 coords, glm::vec3 color) {
            points.push_back(coords);
            colors.push_back(color);
            drawing_type = GL_LINE_STRIP;
        }
        bool primitive_is_finished() {
            return points.size() > 1;
        }
        static std::string get_string_name() {
            return "Line";
        }

        glm::vec3 center() {
            float x = 0, y = 0, z = 0;
            auto res = glm::vec3(0.0f);
            for (auto point : points) {
                res += point;
            }
            size_t size = points.size();
            res /= size;
            return res;
        }

        Line copy() {
            auto res = Line(points[0], colors[0]);
            for (size_t i = 1; i < points.size(); i++) {
                res.push_point(points[i], colors[i]);
            }
            return res;
        }

        void transform(std::function<glm::vec3(glm::vec3)> transofrmator) {
            for (auto i = 0; i < points.size(); i++) {
                points[i] = transofrmator(points[i]);
            }
        }

        void transform(const glm::mat4x4& transform_matrix) {
            this->transform([transform_matrix](glm::vec3 p)->glm::vec3 {
                return transform_matrix * glm::vec4(p.x, p.y, p.z, 1);
                });
        }
    };

    struct Segment : public Primitive {
    public:
        Segment(glm::vec3 left_point, glm::vec3 right_point, glm::vec3 color) {
            points.push_back(left_point);
            points.push_back(right_point);
            drawing_type = GL_LINES;
            colors.push_back(color);
            Type type = segment;
        }
        glm::vec3 left() {
            return points[0];
        }

        glm::vec3 right() {
            return points[1];
        }
        bool primitive_is_finished() {
            return points.size() == 2;
        }
        static std::string get_string_name() {
            return "Segment";
        }
    };

    struct Polygon : public Primitive {
    public:
        glm::vec3 normal;
        Polygon() {}
        Polygon(glm::vec3 coords, glm::vec3 color) {
            points.push_back(coords);
            colors.push_back(color);
            drawing_type = GL_POINTS;
            Type type = polygon;
        }
        void push_point(glm::vec3 coords) {
            points.push_back(coords);
            if (points.size() > 1) {
                drawing_type = GL_LINE_STRIP;
            }
        }
        void push_point(glm::vec3 coords, glm::vec3 color) {
            colors.push_back(color);
            push_point(coords);
        }
        bool primitive_is_finished() {
            if (points.size() < 3) {
                return false;
            }
            drawing_type = GL_LINE_LOOP;
            calc_normal();
            return true;
        }
        bool calc_normal() {
            if (points.size() < 3) {
                return false;
            }
            auto v1 = points[1] - points[0];
            auto v2 = points[points.size() - 1] - points[0];
            normal = glm::normalize(glm::cross(v1, v2));
        }
        static std::string get_string_name() {
            return "Polygon";
        }
    };
}