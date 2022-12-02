#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
struct Cube {
    /**
    // [0] front left top
    // [1] front left bottom
    // [2] front right bottom
    // [3] front right top
    // [4] back left top
    // [5] back left bottom
    // [6] back right bottom
    // [7] back right top
     */
    glm::vec3 vertexes[8];
    static constexpr GLuint indexes[36] = {
        0, 1, 2, // front left
        0, 2, 3, // front right
        4, 5, 1, // left left
        4, 1, 0, // left right
        7, 6, 2, // right left
        7, 2, 3, // right right
        4, 5, 6, // back left
        4, 6, 7, // back right
        4, 0, 3, // top lefts
        4, 3, 7, // top left
        5, 1, 2, // bottom left
        5, 2, 6, // bottom right
    };
    static constexpr GLuint texture_indexes[] = {
            0,2,3,
            3,1,0,
            0,2,3,
            3,1,0,
            2,3,1,
            1,0,2,
            2,3,1,
            1,0,2,
            1,3,2,
            2,0,1,
            1,3,2,
            2,0,1,
    };
private:
    void standart_constructor_init(glm::vec3 flt, glm::vec3 brb) {
        vertexes[0] = flt;
        vertexes[1] = glm::vec3(flt.x, brb.y, flt.z);
        vertexes[2] = glm::vec3(brb.x, brb.y, flt.z);
        vertexes[3] = glm::vec3(brb.x, flt.y, flt.z);
        vertexes[4] = glm::vec3(flt.x, flt.y, brb.z);
        vertexes[5] = glm::vec3(flt.x, brb.y, brb.z);
        vertexes[6] = brb;
        vertexes[7] = glm::vec3(brb.x, flt.y, brb.z);
    }
public:
    Cube(glm::vec3 flt, glm::vec3 brb) {
        standart_constructor_init(flt, brb);
    }
    Cube(GLfloat side_len, glm::vec3 center = glm::vec3(0.0, 0.0, 0.0)) {
        GLfloat half_len = side_len / 2;
        glm::vec3 flt = glm::vec3(center.x - half_len, center.y + half_len, center.z + half_len);
        glm::vec3 brb = glm::vec3(center.x + half_len, center.y - half_len, center.z - half_len);
        standart_constructor_init(flt, brb);
    }
};