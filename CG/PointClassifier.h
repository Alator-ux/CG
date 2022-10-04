#pragma once

#include "Primitives.h"
#include "GLM/glm.hpp"
#define left_from_edge 0;
#define right_from_edge 1;
#define on_the_edge 1;
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
        double rot = ab[0] * bc[1] - ab[1] * bc[0]; // rotation direction
        tempSign = rot < 0;
        if (sign != tempSign) // если поворот отличается от остальных
            return false;
        return true;

    }
public:
    PointClassifier() {};

    int classify_point(Point point, Edge edge) {
        auto p = point.points[0];
        auto edge_p1 = edge.points[0];
        auto edge_p2 = edge.points[1];
        auto point_class = (p[0] - edge_p1[0]) * (edge_p1[1] - edge_p2[1])
            - (p[1] - edge_p1[1]) * (edge_p1[0] - edge_p2[0]);
        if (point_class > 0) {
            return left_from_edge;
        }
        if (point_class < 0) {
            return right_from_edge;
        }
        return on_the_edge;
    }
    bool point_in_polygon(Point point, Polygon pol) {
        auto p = point.points[0];
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
            if (p[1] > std::min(p1[1], p2[1])// Y контрольной точки больше минимума Y конца отрезка
                && p[1] <= std::max(p1[1], p2[1]))// Y контрольной точки меньше максимального Y конца отрезка
            {
                if (p[0] <= std::max(p1[0], p2[0]))// X контрольной точки меньше максимального X конечной точки сегмента изолинии (для оценки используйте левый луч контрольной точки).
                {
                    if (p1[1] != p2[1])// Отрезок не параллелен оси X
                    {
                        x_intersects = (p[1] - p1[1]) * (p2[0] - p1[0]) / (p2[1] - p1[1]) + p1[0];
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