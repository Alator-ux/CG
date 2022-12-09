#pragma once
#include <GLM/glm.hpp>

struct RMaterial
{
public:
    float shininess;
    float specular;
    float diffuse;
    float ambient;
    float tranparrency;
    float reflectivity;

    RMaterial(float sh, float sp, float dif, float amb, float transp, float refl)
    {
        this->shininess = sh;
        this->specular = sp;
        this->diffuse = dif;
        this->ambient = amb;
        this->tranparrency = transp;
        this->reflectivity = refl;
    }
    RMaterial()
    {
    }
};

struct RLight
{
public:
    glm::vec3 source;
    float intensity;
    RLight() {}
    RLight(glm::vec3 source, float intensity)
    {
        this->source = source;
        this->intensity = intensity;
    }
};

class RayTraceObjectInterface
{
public:
    virtual glm::vec3 get_center() = 0;
    virtual glm::vec3 get_color() = 0;
    virtual RMaterial get_material() = 0;

    virtual std::tuple<bool, glm::vec3, glm::vec3> find_intersection(glm::vec3 direction, glm::vec3 originPoint) = 0;
};

glm::vec3 Rnormalize(glm::vec3 source)
{
    float norm = glm::sqrt(glm::dot(source, source));
    return source / norm;
}
glm::vec3 pointProjection(glm::vec3 start, glm::vec3 dir, glm::vec3 proj)
{
    auto param = glm::dot(dir, proj - start) / glm::dot(dir, dir);
    return (start + dir) * param;
}

class RayTracer
{

    glm::vec3 mixColor(glm::vec3 c1, glm::vec3 c2, float ratio)
    {
        return (c2 * ratio) + (c1 * (1 - ratio));
    }

    glm::vec3 computeColorIntens(glm::vec3 color, float intens)
    {
        return {
            int(color.r * intens) % 256,
            int(color.g * intens) % 256,
            int(color.b * intens) % 256};
    }

    glm::vec3 viewReflectionRay(glm::vec3 ray, glm::vec3 normale)
    {
        return Rnormalize(
            2 * (glm::dot((ray * -1.f), normale)) * normale - (-1.f * ray));
    }
    float computeLightness(glm::vec3 intersect, glm::vec3 normale, glm::vec3 viewRay, RMaterial material)
    {
        float diffuse = 0;
        float specular = 0;
        float ambient = 1;
        auto shadowRay = Rnormalize(light.source - intersect);
        auto reflectionRay = viewReflectionRay(shadowRay, normale);
        if (hasIntersection(shadowRay, intersect))
        {
            return 0;
        }
        diffuse = light.intensity * glm::max(glm::dot(shadowRay, normale), 0.f);
        specular = light.intensity * glm::pow(
                                         glm::max(glm::dot(reflectionRay, (viewRay * -1.f)), 0.f), material.shininess);
        return ambient * material.ambient + diffuse * material.diffuse + specular * material.specular;
    }

    bool hasIntersection(glm::vec3 direction, glm::vec3 origin)
    {
        bool hasIntersection;
        glm::vec3 intersect;
        glm::vec3 normale;
        for (auto fig : scene)
        {
            std::tie(hasIntersection, intersect, normale) = fig->find_intersection(direction, origin);
            if (hasIntersection)
            {
                return true;
            }
        }
        return false;
    }

public:
    std::vector<RayTraceObjectInterface *> scene;
    RLight light;
    glm::vec3 ray;
    RayTracer(
        std::vector<RayTraceObjectInterface *> scene,
        RLight light,
        glm::vec3 ray)
    {
        this->scene = scene;
        this->ray = ray;
        this->light = light;
    }

    glm::vec3 shootRay(glm::vec3 ray, glm::vec3 origin, int depth)
    {
        float nearest = 1e6;

        if (depth >= 5)
        {
            return {128, 128, 128};
        }
        glm::vec3 res = {0, 0, 0};
        bool hasIntersection;
        glm::vec3 intersect;
        glm::vec3 normale;
        for (auto figure : scene)
        {
            std::tie(hasIntersection, intersect, normale) = figure->find_intersection(ray, origin);
            if (!hasIntersection)
            {
                continue;
            }
            if (intersect.z >= nearest)
            {
                continue;
            }
            nearest = intersect.z;
            res = computeColorIntens(figure->get_color(), computeLightness(intersect, normale, ray, figure->get_material()));
            if (figure->get_material().reflectivity > 0)
            {
                auto reflectedColor = shootRay(viewReflectionRay(ray, normale), intersect, depth + 1);
                res = mixColor(res, reflectedColor, figure->get_material().reflectivity);
            }
        }
        return res;
    }
};