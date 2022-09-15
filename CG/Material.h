#pragma once
#include <GLFW/glfw3.h>
#include <SFML/Graphics/Texture.hpp>
#include <glm/glm.hpp>
struct Material {
    //sf::Texture texture;
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emission;
    GLfloat shininess;
    GLfloat rougless;
    Material(glm::vec3 color = glm::vec3(1.0), glm::vec3 ambient = glm::vec3(0.2),
        glm::vec3 diffuse = glm::vec3(1.0), glm::vec3 specular = glm::vec3(1.0),
        glm::vec3 emission = glm::vec3(0.0), GLfloat shininess = 16.0f,
        GLfloat roughless = 0.3) {
        this->color = color;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->emission = emission;
        this->shininess = shininess;
        this->rougless = roughless;
    }
    std::string get_color_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "materialColor" + suf;
    }
    std::string get_ambient_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "materialAmbient" + suf;
    }
    std::string get_dif_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "materialDiffuse" + suf;
    }
    std::string get_spec_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "materialSpec" + suf;
    }
    std::string get_emission_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "materialEmission" + suf;
    }
    std::string get_shininess_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "materialShininess" + suf;
    }
    std::string get_roughless_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "materialRoughness" + suf;
    }
};