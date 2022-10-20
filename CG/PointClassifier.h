#pragma once

#include "GL/glew.h"
#include "Primitives.h"
#include "GLM/vec3.hpp"
#include "PrimitiveChanger.h"
#define left_from_edge "Point is left from edge";
#define right_from_edge "Point is right from edge";
#define on_the_edge "Point on the edge";
class PointClassifier {

    bool is_polygon_convex(Polygon pol) {
        auto points = pol.points;
        if (points.size() < 3) {
            return false;
        }
        
        glm::vec3 ab = glm::vec3(points[1][0] - points[0][0], points[1][1] - points[0][1],1);
        glm::vec3 bc = glm::vec3(points[2][0] - points[1][0], points[2][1] - points[1][1],1);
        double rot = ab[0] * bc[1] - ab[1] * bc[0]; // rotation direction
        bool sign = rot < 0;
        bool tempSign = sign;
        for (int i = 1; i < points.size() - 1; i++)
        {
            ab = glm::vec3(points[i][0] - points[i - 1][0], points[i][1] - points[i - 1][1], 1);
            bc = glm::vec3(points[i + 1][0] - points[i][0], points[i + 1][1] - points[i][1],1);
            rot = ab[0] * bc[1] - ab[1] * bc[0];
            tempSign = rot < 0;
            if (sign != tempSign) {// если поворот отличается от остальных
                return false;
            }
        }
        // checking last and first points
        ab = glm::vec3(points[0][0] - points[points.size() - 1][0], points[0][1] - points[points.size() - 1][1], 1);
        bc = glm::vec3(points[1][0] - points[0][0], points[1][1] - points[0][1], 1);
        rot = ab[0] * bc[1] - ab[1] * bc[0]; // rotation direction
        tempSign = rot < 0;
        if (sign != tempSign) // если поворот отличается от остальных
            return false;
        return true;

    }
public:
    PointClassifier() {};

    std::string classify(Primitive* item, glm::vec3 point) {
        if (item == nullptr) {
            return "Nothing to classify";
        }
        std::string str;
        if (item->type == 1) {
            Edge* edge = reinterpret_cast<Edge*>(item);
            return classify_point(point, *edge);
        }
        else if (item->type == 2) {
            Polygon* poly = reinterpret_cast<Polygon*>(item);
            bool pip_install = point_in_polygon(point, *poly);
            if (pip_install) {
                return "Point in the polygon";
            }
            return "Point not in the polygon";
        }
        else {
            return "Nothing to classify";
        }
    }

    std::string classify_point(glm::vec3 p, Edge edge) {
        auto edge_p1 = edge.points[0];
        auto edge_p2 = edge.points[1];
        auto point_class = (p[0] - edge_p1[0]) * (edge_p1[1] - edge_p2[1])
            - (p[1] - edge_p1[1]) * (edge_p1[0] - edge_p2[0]);
        if (point_class > 0) {
            return right_from_edge;
        }
        if (point_class < 0) {
            return left_from_edge;
        }
        return on_the_edge;
    }
    glm::vec3 get_line_function(Edge line) {
        auto p1 = line.points[0];
        auto p2 = line.points[1];
        GLfloat c1 = p1[1] - p2[1];
        GLfloat c2 = p2[0] - p1[0];
        GLfloat c3 = p1[0] * p2[1] - p2[0] * p1[1];
        return glm::vec3(c1, c2, c3);
    }
    glm::vec3 find_intersection(Edge first, Edge second) {
        auto fc = this->get_line_function(first); // first line coefficients
        auto sc = this->get_line_function(second); // second line coefficients
        double delimiter = fc[0] * sc[1] - sc[0] * fc[1];
        if (delimiter == 0)
        {
            return no_intersection;
        }
        auto x = (fc[1] * sc[2] - sc[1] * fc[2]) / delimiter;
        auto y = (fc[2] * sc[0] - sc[2] * fc[0]) / delimiter;

        return glm::vec3(x, y, 1
        );
    }
    bool point_in_polygon(glm::vec3 p, Polygon pol) {
        bool is_convex = this->is_polygon_convex(pol);
        auto points = pol.points;
        int counter = 0;
        double x_intersects;
        int pointCount = points.size();
        glm::vec3 p1 = points[0];
        glm::vec3 p2 = p1;
        for (int i = 1; i <= pointCount; i++)
        {
            p2 = points[i % pointCount]; // типа циклично идём окда

            if (p.y > std::min(p1.y, p2.y)// Y контрольной точки больше минимума Y конца отрезка
                && p[1] <= std::max(p1.y, p2.y))// Y контрольной точки меньше максимального Y конца отрезка
            {
                if (p[0] <= std::max(p1.x, p2.x))// X контрольной точки меньше максимального X конечной точки сегмента изолинии (для оценки используйте левый луч контрольной точки).
                {
                    if (p1.y != p2.y)// Отрезок не параллелен оси X
                    {
                        //counter++;
                        /*double delimiter = fc[0] * sc[1] - sc[0] * fc[1];
                        if (delimiter == 0)
                        {
                            return no_intersection;
                        }
                        auto x = (fc[1] * sc[2] - sc[1] * fc[2]) / delimiter;*/
                        Edge e1 = Edge(p, glm::vec3(1));
                        e1.push_point(glm::vec3(p1.x, p.y, 1.0f));
                        Edge e2 = Edge(p1, glm::vec3(1));
                        e2.push_point(p2);
                        auto x_intersects = find_intersection(e1, e2).x;
                        //x_intersects = (p[1] - p1[1]) * (p2[0] - p1[0]) / (p2[1] - p1[1]) + p1[0];
                        if (p1[0] == p2[0] || p[0 ] <= x_intersects)
                        {
                            counter++;
                        }
                    }
                }

            }
            p1 = p2;
        }
        return counter % 2 == 1;
    }
};