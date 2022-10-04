#pragma once
#include "Primitives.h"
#include "GLM/mat3x3.hpp"
#include "GLM/vec3.hpp"
#include "GLM/glm.hpp"
#include <typeinfo>


const double PI = 3.141592653589793238463;
const int empty_item = -1;

class PrimitiveChanger {
    std::vector<Primitive>* storage;
    int active_item;
    int mode;

    // Converts provided angle to it's radian presentation
    double toRadians(double angle) {
        return PI * angle / 180.0;
    }

    // Returns matrix which needed for point rotation.
    // x and y are the coordinates of point, which we are roating around
    glm::mat3x3 build_rotation_matrix(double x, double y, double angle) {
        auto rad_angle = this->toRadians(angle);
        auto cos_r_a = cos(rad_angle);
        auto sin_r_a = sin(rad_angle);
        glm::mat3x3 shift_matrix1 = glm::mat3x3(
            1, 0, 0,
            0, 1, 0,
            -x, -y, 1);
        glm::mat3x3 shift_matrix2 = glm::mat3x3(
            1, 0, 0,
            0, 1, 0,
            x, y, 1);
        glm::mat3x3 rotation_matrix = glm::mat3x3(
            cos_r_a, -sin_r_a, 0,
            sin_r_a, cos_r_a, 0,
            0, 0, 1);
        return (shift_matrix1 * rotation_matrix * shift_matrix2);
    }

    glm::mat3x3 build_scale_matrix(double x, double y, double x_factor, double y_factor) {
        glm::mat3x3 shift_matrix1 = glm::mat3x3(
            1, 0, 0,
            0, 1, 0,
            -x, -y, 1);
        glm::mat3x3 shift_matrix2 = glm::mat3x3(
            1, 0, 0,
            0, 1, 0,
            x, y, 1);
        glm::mat3x3 scaling_matrix = glm::mat3x3(
            x_factor, 0, 0,
            0, y_factor, 0,
            0, 0, 1);
        return (shift_matrix1 * scaling_matrix * shift_matrix2);
    }
public:
    PrimitiveChanger(std::vector<Primitive>* storage) {
        this->storage = storage;
        this->active_item = -1;
    }

    void shift(int item_index, int dx, int dy) {
        if (item_index == empty_item) {
            return;
        }
        
        glm::mat3x3 shift_matrix = glm::mat3x3(1, 0, 0, 
                                               0, 1, 0, 
                                               dx, dy, 1);
        // TODO: Максим, у меня ощущение, что здесь я копирую. Надо по ссылке тот элемент получать
        auto item = (* storage)[item_index];
        auto points = item.points;
        for (int i = 0; i < points.size(); i++) {
            glm::vec3 values = glm::vec3(points[i][0], points[i][1], 1); //TODO: remove it m8
            auto shifted_point = values * shift_matrix;
            //TODO: Если по ссылке - запарно, то просто тут нужному айтему присваивать
            item.points[i] = shifted_point;
        }
    }
    void rotate_around_point(int item_index, glm::vec3 point, double angle) {
        if (item_index == empty_item) {
            return;
        }
        glm::mat3x3 rotation_matrix = this->build_rotation_matrix(point[0], point[1], angle);
        auto item = (*storage)[item_index];
        auto points = item.points;
        for (int i = 0; i < points.size(); i++) {
            glm::vec3 values = glm::vec3(points[i][0], points[i][1], 1);
            item.points[i] = values * rotation_matrix;
        }
    }

    void scale_from_point(int item_index, glm::vec3 point, double x_factor, double y_factor) {
        if (item_index == empty_item) {
            return;
        }
        auto scale_matrix = this->build_scale_matrix(point[0], point[1], x_factor, y_factor);
        auto item = (*storage)[item_index];
        auto points = item.points;
        for (int i = 0; i < points.size(); i++) {
            glm::vec3 values = glm::vec3(points[i][0], points[i][1], 1);
            //TODO: Если по ссылке - запарно, то просто тут нужному айтему присваивать
            item.points[i] = values * scale_matrix;
        }
    }

    void rotate_edge_90(int item_index) {
        if (item_index == empty_item) {
            return;
        }
        auto edge = (*storage)[item_index];
        if (typeid(edge) != typeid(Edge())) { //TODO: may explode
            return;
        }
        auto p1 = edge.points[0];
        auto p2 = edge.points[1];
        auto mid = glm::vec3(
            (p1[0] + p2[0]) / 2,
            (p1[1] + p2[1]) / 2,
            1
        );
        this->rotate_around_point(item_index, mid, 90.0);
    }
};