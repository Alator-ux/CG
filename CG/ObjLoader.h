#pragma once
#include "Primitives.h"
#include "Figure.h"
#include <sstream>
#include <fstream>
#include <iostream>

struct ObjVertex {
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
};

std::vector<ObjVertex> loadOBJ(const char* file_name) {
    //Vertex portions
    std::vector<glm::fvec3> positions;
    std::vector<glm::fvec2> texcoords;
    std::vector<glm::fvec3> normals;

    //Face vectors
    std::vector<GLint> pos_indicies;
    std::vector<GLint> texcoord_indicies;
    std::vector<GLint> normal_indicies;

    //Vertex array
    std::vector<ObjVertex> vertices;

    std::stringstream ss;
    std::ifstream in_file(file_name);
    std::string line = "";
    std::string prefix = "";
    glm::vec3 temp_vec3;
    glm::vec2 temp_vec2;
    GLint temp_glint = 0;

    if (!in_file.is_open()) {
        throw "ERROR::OBJLOADER::Could not open file.";
    }

    while (std::getline(in_file, line)) {
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "#") {

        }
        else if (prefix == "o") {

        }
        else if (prefix == "s") {

        }
        else if (prefix == "use_mtl") {

        }
        else if (prefix == "v") {
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            positions.push_back(temp_vec3);
        }
        else if (prefix == "vt") {
            ss >> temp_vec2.x >> temp_vec2.y;
            texcoords.push_back(temp_vec2);
        }
        else if (prefix == "vn") {
            ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
            normals.push_back(temp_vec3);
        }
        else if (prefix == "f") {
            int counter = 0;
            while (ss >> temp_glint)
            {
                if (counter == 0) {
                    pos_indicies.push_back(temp_glint);
                }
                else if (counter == 1) {
                    texcoord_indicies.push_back(temp_glint);
                }
                else if (counter == 2) {
                    normal_indicies.push_back(temp_glint);
                }

                if (ss.peek() == '/') {
                    ++counter;
                    ss.ignore(1, '/');
                }
                else if (ss.peek() == ' ') {
                    ++counter;
                    ss.ignore(1, ' ');
                }

                if (counter > 2)
                    counter = 0;
            }
        }
        else {

        }
    }

    //Build final vertex array (mesh)
    vertices.resize(pos_indicies.size(), ObjVertex());

    //Load in all indices
    for (size_t i = 0; i < pos_indicies.size(); ++i) {
        vertices[i].position = positions[pos_indicies[i] - 1];
        if (texcoords.size() != 0)
        {
            vertices[i].texcoord = texcoords[texcoord_indicies[i] - 1];
        }
        vertices[i].normal = normals[normal_indicies[i] - 1];
    }

    std::cout << "Nr of vertices: " << vertices.size() << "\n";
    std::cout << "OBJ file loaded!" << "\n";
    return vertices;
}