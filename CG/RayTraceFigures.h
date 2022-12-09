#pragma once
#include "RayTraceTools.h"
#include "RayTraceFace.h"
#include <vector>
#include<tuple>

class RSphere : virtual public RayTraceObjectInterface {
    glm::vec3 center;
    glm::vec3 color;
    float radius;
    RMaterial material;
public:
    RSphere(glm::vec3 center, float radius, glm::vec3 color, RMaterial material){
        this->center = center;
        this->radius = radius;
        this->color = color;
        this->material = material;
    }
    glm::vec3 get_center() override {
        return this->center;
    }
    glm::vec3 get_color() override {
        return this->color;
    }
    RMaterial get_material() override
    {
        return this->material;
    };

    std::tuple<bool, glm::vec3, glm::vec3> find_intersection(glm::vec3 direction, glm::vec3 originPoint) override {
        direction = Rnormalize(direction);
        auto fromSourceToCenter = this->center - originPoint;
        if (glm::dot(direction, fromSourceToCenter) < 0) // Sphera's center is behind ray's point
        {
            auto dist = glm::distance(originPoint, this->center);
            auto position = dist - this->radius;
            if (position > 0 || position < 1e-5) { // first case - no intersection; second - we are on the sphere
                return { false, {}, {} };
            }
            auto projectedPoint = pointProjection(originPoint, direction, center);
            float distance = glm::sqrt(glm::pow(radius, 2) - glm::pow(glm::distance(center, projectedPoint), 2)) - glm::distance(originPoint, projectedPoint);
            auto intersect = (originPoint + direction) * distance;
            return { true, intersect, Rnormalize((intersect - center)) };
        }
        // otherwise - we can project sphera's center to ray
        auto projectedPoint = pointProjection(originPoint, direction, center);
        float distance = glm::distance(center, projectedPoint);
        if ( distance > radius) { // out of bounds
            return { false, {}, {} };
        }

        distance = glm::sqrt(glm::pow(radius, 2) - glm::pow(distance, 2));
        if (glm::distance(originPoint, center) > radius) {
            distance = glm::distance(originPoint, projectedPoint) - distance;
        }
        else {
            distance = glm::distance(originPoint, projectedPoint) + distance;
        }
        auto intersect = (originPoint + direction) * distance;
        return { true, intersect, Rnormalize((intersect - center)) };
    }
};

class RCube : virtual public RayTraceObjectInterface {
    glm::vec3 center;
    glm::vec3 color;
    std::vector<RFace> faces;
    RMaterial material;
public:
    RCube(glm::vec3 center, float side, glm::vec3 color, RMaterial material) {
        this->center = center;;
        this->color = color;
        this->material = material;
        auto hside = side / 2;
        faces = std::vector<RFace>{
            RFace(
                {center.x, center.y, center.z - hside},
                {0,0,-1},
                {0,1,0},
                side, side,
                color, material
                ),
            RFace(
                {center.x, center.y, center.z + hside},
                {0,0,1},
                {0,1,0},
                side, side,
                color, material
                ),
            RFace(
                {center.x, center.y - hside, center.z},
                {0,-1,0},
                {0,0,1},
                side, side,
                color, material
                ),
            RFace(
                {center.x, center.y + hside, center.z},
                {0,1,0},
                {0,0,1},
                side, side,
                color, material
                ),
            RFace(
                {center.x + hside, center.y, center.z},
                {1,0,0},
                {0,1,0},
                side, side,
                color, material
                ),
            RFace(
                {center.x - hside, center.y, center.z},
                {-1,0,0},
                {0,1,0},
                side, side,
                color, material
                )
        };
    };
    glm::vec3 get_center() override {
        return this->center;
    };
    glm::vec3 get_color() override {
        return this->color;
    };
    RMaterial get_material() override
    {
        return this->material;
    };

    std::tuple<bool, glm::vec3, glm::vec3> find_intersection(glm::vec3 direction, glm::vec3 originPoint) override {
        float nearestPoint = 1e6;
        std::tuple<bool, glm::vec3, glm::vec3> res;
        bool hasIntersection;
        glm::vec3 intersect;
        glm::vec3 normale;
        for (auto face : faces) {

            std::tie(hasIntersection, intersect, normale) = face.find_intersection(direction, originPoint);
            if (!hasIntersection) {
                continue;
            }

            auto tNearest = glm::distance(originPoint, intersect);
            if (tNearest > nearestPoint) {
                continue;
            }

            nearestPoint = tNearest;
            res = { hasIntersection, intersect, normale };
        }
        return res;
    };
};

class RRoom : virtual public RayTraceObjectInterface {
    float side;
    glm::vec3 center;
    glm::vec3 color;
    std::vector<RFace> walls;
    RMaterial material;
public:
    RRoom(glm::vec3 center, float side, RMaterial lM, RMaterial rM, RMaterial fM, RMaterial bM, RMaterial flM, RMaterial cM) {
        this->center = center;
        this->side = side;
        auto leftWall = RFace(
            { center.x - side / 2, center.y, center.z },
            { 1,0,0 },
            { 0,1,0 },
            side, side,
            { 199,21,133 }, // Medium violet red
            lM
            );
        auto rightWall = RFace(
            { center.x + side / 2, center.y, center.z },
            { -1,0,0 },
            { 0,1,0 },
            side, side,
            { 32, 178, 170}, // Light Sea Green
            rM
        );
        auto frontWall = RFace(
            { center.x, center.y, center.z - side / 2 },
            { 0, 0, 1 },
            { 0, 1, 0 },
            side, side,
            { 147, 112, 219 }, // Medium purple
            fM
        );
        auto backWall = RFace(
            { center.x, center.y, center.z + side / 2 },
            { 0, 0, -1 },
            { 0, 1, 0 },
            side, side,
            { 255, 255, 0}, // Yellow
            bM
        );
        auto floor = RFace(
            { center.x, center.y - side / 2, center.z},
            { 0, 1, 0 },
            { 0, 0, 1 },
            side, side,
            { 30, 144, 255 }, // DodgerBlue
            flM
        );
        auto ceil = RFace(
            { center.x, center.y + side / 2, center.z },
            { 0, -1, 0 },
            { 0, 0, 1 },
            side, side,
            { 255, 218, 185 }, // PeachPuff
            flM
        );
        this->walls = { 
            leftWall, 
            rightWall, 
            floor, 
            ceil, 
            frontWall, 
            backWall 
        };
    }
    glm::vec3 get_center() override {
        return this->center;
    };
    glm::vec3 get_color() override {
        return this->color;
    };
    RMaterial get_material() override
    {
        return this->material;
    };

    std::tuple<bool, glm::vec3, glm::vec3> find_intersection(glm::vec3 direction, glm::vec3 originPoint) override {
        float nearestPoint = 1e6;
        std::tuple<bool, glm::vec3, glm::vec3> res;
        bool hasIntersection;
        glm::vec3 intersect;
        glm::vec3 normale;
        for (auto face : walls) {

            std::tie(hasIntersection, intersect, normale) = face.find_intersection(direction, originPoint);
            if (!hasIntersection) {
                continue;
            }

            auto tNearest = intersect.z;
            if (tNearest > nearestPoint) {
                continue;
            }
            res = { hasIntersection, intersect, normale };
            this->color = face.color;
            this->material = face.material;
        }
        return res;
    };
};

