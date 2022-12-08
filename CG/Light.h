#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>

struct LightSource {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 attenuation;
    LightSource(glm::vec3 position = glm::vec3(0.0), glm::vec3 ambient = glm::vec3(1.0),
        glm::vec3 diffuse = glm::vec3(1.0), glm::vec3 specular = glm::vec3(1.0),
        glm::vec3 attenuation = glm::vec3(1.0, 0.22, 0.20)) {
        this->position = position;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->attenuation = attenuation;
    }
    void set_atten_zero() {
        attenuation = glm::vec3(1.0, 0.0, 0.0);
    }
    std::string get_pos_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "pos" + suf;
    }
    std::string get_ambient_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "ambient" + suf;
    }
    std::string get_dif_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "diffuse" + suf;
    }
    std::string get_spec_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "specular" + suf;
    }
    std::string get_atten_name(const std::string& pref = "", const std::string& suf = "") const {
        return pref + "atten" + suf;
    }
};