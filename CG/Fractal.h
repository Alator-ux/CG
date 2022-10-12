#pragma once
#include <stack>
#include "GL/glew.h"
#include "PrimitiveChanger.h"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

class Fractal {
    std::vector<Primitive> edges;
    std::vector<std::string> l_system;
    float rnd_from;
    float rnd_to;
    std::string axiom;
    float rotation;
    std::string dir;
    glm::vec3 start_p;
    glm::vec3 end_p;
    int generation;
private:
    std::vector<std::string> split_str(const std::string& str) {
        std::vector<std::string> res;
        std::stringstream ss(str);
        std::string word;
        
        while (ss >> word) {
            res.push_back(word);
        }

        return res;
    }
    int rand(int from, int to) {
        if (to - from == 0) {
            return 0;
        }
        return from + std::rand() % (to - from);
    }
    glm::vec3 find_max_by(int ind) {
        auto max_p = glm::vec3(-1);
        for (Primitive& edge : edges) {
            if (edge.points[1][ind] > max_p[ind]) {
                max_p = edge.points[1];
            }
        }
        return max_p;
    }
    glm::vec4 find_min_max() {
        glm::vec3& fp = edges[0].points[0];
        auto min_max = glm::vec4(fp.x, fp.y, fp.x, fp.y);

        for (Primitive& edge : edges) {
            if (edge.points[1].x < min_max[0]) {
                min_max[0] = edge.points[1].x;
            }
            if (edge.points[1].y < min_max[1]) {
                min_max[1] = edge.points[1].y;
            }
            if (edge.points[1].x > min_max[2]) {
                min_max[2] = edge.points[1].x;
            }
            if (edge.points[1].y > min_max[3]) {
                min_max[3] = edge.points[1].y;
            }
        }
        return min_max;
    }
    void scale() {
        auto pr = PrimitiveChanger();

        glm::vec4 min_max = find_min_max();
        glm::vec3 frac_center = glm::vec3(min_max[0] + (min_max[2] - min_max[0]) / 2,
            min_max[1] + (min_max[3] - min_max[1]) / 2, 1);
        auto center = glm::vec3(start_p.x + (end_p.x - start_p.x) / 2,
            start_p.y, 1);
        auto dx = center.x - frac_center.x;
        auto dy = center.y - frac_center.y;
        min_max[0] += dx;
        min_max[2] += dx;
        min_max[1] += dy;
        min_max[3] += dy;
        frac_center.x += dx;
        float temp = center.x - min_max[0];
        auto x_factor = (end_p.x - center.x);
        if (temp != 0) {
            x_factor /= temp;
        }
        temp = center.y - min_max[1];
        auto y_factor = (end_p.y - center.y);
        if (temp != 0) {
            y_factor /= temp;
        }
        
        
        //auto dy 
        float step;
        if (x_factor < y_factor) {
            for (Primitive& edge : edges) {
                pr.shift(&edge, dx, dy);
                pr.scale_from_point(&edge, center, x_factor, x_factor);
                //auto dx = abs(edge.points[0].x - start_p.x);
            }
        }
        else {
            for (Primitive& edge : edges) {
                pr.shift(&edge, dx, dy);
                pr.scale_from_point(&edge, center, y_factor, y_factor);
                //auto dx = abs(edge.points[0].x - start_p.x);
            }
        }
    }
public:
    Fractal(int type, int rnd_from, int rnd_to, 
            glm::vec3 start_p, glm::vec3 end_p) {
        set_fractal_type(type);

        this->rnd_from = rnd_from;
        this->rnd_to = rnd_to;
        this->start_p = start_p;
        this->end_p = end_p;
        this->generation = 1;
    }

    void generate() {
        edges.clear();
        auto rules = std::map<char, std::string>();
        for (auto line : l_system) {
            rules[line[0]] = line.substr(2);
        }

        auto rule = axiom;

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

        auto pr = PrimitiveChanger();
        auto r_rot = pr.toRadians(rotation);

        std::stack<std::pair<glm::vec3, float>> states;
        glm::vec3 point = start_p;

        for (char lex : rule) {
            if (lex == 'F') {
                float x = point.x + cos(angle);
                float y = point.y + sin(angle);
                auto new_p = glm::vec3(x, y, 1);
                edges.push_back(Edge(point, new_p, glm::vec3(1)));
                point = new_p;
            } 
            else if (lex == '[') {
                auto state = std::pair<glm::vec3, float>(point, angle);
                states.push(state);
            }
            else if (lex == ']') {
                auto state = states.top();
                states.pop();
                point = state.first;
                angle = state.second;
            }
            else if (lex == '+') {
                int noise = rand(rnd_from, rnd_to);
                angle += r_rot + pr.toRadians(noise);
            }
            else if (lex == '-') {
                int noise = rand(rnd_from, rnd_to);
                angle -= r_rot + pr.toRadians(noise);
            }
        }

        scale();
    }

    void set_fractal_type(int type) {
        std::string fname = "";
        switch (type)
        {
        case 0:
        {
            fname = "./fractals/koch_curve.txt";
            break;
        }
        case 1:
        {
            fname = "./fractals/koch_snowflake.txt";
            break;
        }
        case 2:
        {
            fname = "./fractals/serpinsky_triangle.txt";
            break;
        }
        case 3:
        {
            fname = "./fractals/serpinsky_carpet.txt";
            break;
        }
        case 4:
        {
            fname = "./fractals/high_tree.txt";
            break;
        }
        default:
            break;
        }

        l_system.clear();
        std::ifstream file;
        file.open(fname);
        std::string line;

        if (file) {
            std::getline(file, line);
            auto f_line = split_str(line);
            this->axiom = f_line[0];
            this->rotation = std::atof(f_line[1].c_str());
            this->dir = f_line[2];
        }

        std::getline(file, line);
        while (file) {
            l_system.push_back(line);
            std::getline(file, line);
        }

        file.close();
    }

    void set_generation(int generation) {
        this->generation = generation;
    }

    std::vector<Primitive>& get_items() {
        return this->edges;
    }
};