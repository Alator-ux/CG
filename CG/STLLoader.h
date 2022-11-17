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
        bool suc = false;
        while (std::getline(stl_file, str))
        {
            std::stringstream iss;
            iss << str;
            std::string word;
            
            std::getline(iss, word, ' ');
            if (word == "facet") {
                suc = true;
                std::getline(iss, word, ' ');
                for (size_t i = 0; i < 3; i++) {
                    std::getline(iss, word, ' ');
                    out_face.normal[i] = std::atof(word.c_str());
                }
            }
            else if (word == "vertex") {
                glm::vec3 coord;
                for (size_t i = 0; i < 3; i++) {
                    std::getline(iss, word, ' ');
                    coord[i] = std::atof(word.c_str());
                }
                out_face.push_point(coord);
            }
            else if (word == "endfacet") {
                break;
            }
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
            std::getline(stl_file, word);
            while (true) {
                auto p = read_part(stl_file);
                if (!p.first) {
                    break;
                }
                p.second.colors.push_back(glm::vec3(1.0));
                stl_model.objects.push_back(p.second);
            }
            stl_file.close();
        }
        else std::cout << "Unable to open file";

        return stl_model;
    }

    static void save_to_file(const std::string& path, const HighLevelInterface& data) {
        std::ofstream stl_file(path);
        stl_file << "solid Chernovilkin\n";
        for (auto& obj : data.objects) {
            const primitives::Polygon* poly = reinterpret_cast<const primitives::Polygon*>(&obj);
            stl_file << "facet normal " << poly->normal.x << " " 
                << poly->normal.y << " " << poly->normal.z << std::endl;
            stl_file << "outer loop\n";
            for (auto& point : poly->points) {
                stl_file << "vertex " << point.x << " " << point.y << " " << point.z << std::endl;
            }
            stl_file << "endloop\n";
            stl_file << "endfacet\n";
        }
        stl_file << "endsolid Chernovilkin\n";
    }
};