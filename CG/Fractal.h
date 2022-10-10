#pragma once
#include "GL/glew.h"
#include "PrimitiveChanger.h"
#include <map>
#include <string>
#include <vector>
#include <sstream>

class Fractal {
    std::vector<glm::vec3> points;
    std::vector<std::string> l_system;
    size_t generation;
    float from;
    float to;
public:
    Fractal(const char* fname) {

    }
    void generate(std::string rule, float rotation, const std::string& dir) {
        auto rules = std::map<char, std::string>();
        for (auto line : l_system) {
            rules[line[0]] = line.substr(2);
        }

        for (size_t i = 0; i < generation; i++) {
            std::stringstream ss;
            for (auto lex : rule) {
                if (rules.count(lex)) {
                    ss << rules[lex];
                }
                else {
                    ss << lex;
                }
            }
            rule = ss.str();
        }

        float angle = 0;
        if (dir == "left") {
            angle = PI;
        }
        else if (dir == "right") {
            angle = 0;
        } else if (dir == "up") {
            angle = 3 * PI / 2; // 270 / -90 градусов
        }

        rotation = PrimitiveChanger().toRadians(rotation);
    }
};