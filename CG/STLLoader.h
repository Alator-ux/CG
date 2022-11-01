#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Figure.h"
#include "Primitives.h"

class STL {
    static std::pair<bool, primitives::Primitive> read_part(std::ifstream& stl_file) {
        std::string str;
        auto out_face = Face();
        std::string next_key_word = "outer loop";
        while (std::getline(stl_file, str))
        {
            if (str == next_key_word) {
                next_key_word = "vertex";
                break;
            }
        }
        bool suc = false;
        while (std::getline(stl_file, str))
        {
            if (str == "endloop") {
                break;
            }
            std::stringstream iss;
            iss << str;
            std::string word;
            while (std::getline(iss, word, ' '))
            {
                if (word == next_key_word) {
                    suc = true;
                    glm::vec3 coord;
                    for (size_t i = 0; i < 3; i++) {
                        std::getline(iss, word, ' ');
                        coord[i] = std::atof(word.c_str());
                    }
                    out_face.push_point(coord);
                }
            }
            iss.clear();
        }
        return { suc, out_face };
    }
public:

    static Figure load_from_file(const std::string& path) {
        std::ifstream stl_file(path);
        std::string word;
        std::stringstream ss;
        Figure stl_model;
        if (stl_file.is_open())
        {
            while (true) {
                auto p = read_part(stl_file);
                if (!p.first) {
                    break;
                }
                p.second.color = glm::vec3(1.0);
                stl_model.objects.push_back(p.second);
            }
            stl_file.close();
        }
        else std::cout << "Unable to open file";

        return stl_model;
    }

    static void save_to_file(const std::string& path, const HighLevelInterface& data) {
        std::ofstream stl_file(path);
        for (auto& obj : data.objects) {
            stl_file << "outer loop\n";
            for (auto& point : obj.points) {
                stl_file << "vertex " << point.x << " " << point.y << " " << point.z << std::endl;
            }
            stl_file << "endloop\n";
        }
    }
};