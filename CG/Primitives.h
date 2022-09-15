#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
struct TwoDVertex {
    GLfloat x;
    GLfloat y;
private:
    bool coordIsCorrect(GLfloat c) {
        return -1.0 <= c && c <= 1.0;
    }
public:
    TwoDVertex() {
        x = 0.0;
        y = 0.0;
    }
    TwoDVertex(GLfloat x, GLfloat y) {
        if (!coordIsCorrect(x)) {
            // ошибка
        }
        if (!coordIsCorrect(y)) {
            // ошибка
        }
        this->x = x;
        this->y = y;
    }
    friend inline bool operator==(const TwoDVertex& lhs, const TwoDVertex& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    friend inline bool operator<(const TwoDVertex& lhs, const TwoDVertex& rhs) {
        return lhs.x < rhs.x&& lhs.y < rhs.y;
    }
    friend inline bool operator>(const TwoDVertex& lhs, const TwoDVertex& rhs) {
        return lhs.x > rhs.x && lhs.y > rhs.y;
    }
    friend inline bool operator>=(const TwoDVertex& lhs, const TwoDVertex& rhs) {
        return lhs > rhs || lhs == rhs;
    }
    friend inline bool operator<=(const TwoDVertex& lhs, const TwoDVertex& rhs) {
        return lhs < rhs || lhs == rhs;
    }
    bool lessY(const TwoDVertex& other) {
        return y < other.y;
    }
    bool lessX(const TwoDVertex& other) {
        return x < other.x;
    }
};
struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
    Color() {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 0.0f;
    }
    Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    Color(const GLfloat* rgba) {
        this->r = rgba[0];
        this->g = rgba[1];
        this->b = rgba[2];
        this->a = rgba[3];
    }
    glm::vec3 to_vec3() {
        return glm::vec3(r, g, b);
    }
    glm::vec4 to_vec4() {
        return glm::vec4(r, g, b, a);
    }
    static Color red() {
        return Color(1.0f, 0.0f, 0.0f, 1.0f);
    }
    static Color green() {
        return Color(0.0f, 1.0f, 0.0f, 1.0f);
    }
    static Color blue() {
        return Color(0.0f, 0.0f, 1.0f, 1.0f);
    }
    static Color white() {
        return Color(1.0f, 1.0f, 1.0f, 1.0f);
    }
    static Color copper() {
        return Color(1.0f, 0.5f, 0.31f, 1.0f);
    }
    static Color dark_olive_green() {
        return Color(0.33f, 0.42f, 0.18f, 1.0f);
    }
};

struct Vertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;

private:
    bool coordIsCorrect(GLfloat c) {
        return -1.0 <= c && c <= 1.0;
    }
public:
    Vertex() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
    Vertex(GLfloat x, GLfloat y, GLfloat z = 0.0) {
        if (!coordIsCorrect(x)) {
            // ошибка
        }
        if (!coordIsCorrect(y)) {
            // ошибка
        }
        if (!coordIsCorrect(z)) {
            // ошибка
        }
        this->x = x;
        this->y = y;
        this->z = z;
    }
    bool equalX(const Vertex& other) {
        return x == other.x;
    }
    bool equalY(const Vertex& other) {
        return y == other.y;
    }
    bool equalZ(const Vertex& other) {
        return z == other.z;
    }
    friend inline bool operator==(const Vertex& lhs, const Vertex& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == lhs.z;
    }
    friend inline bool operator<(const Vertex& lhs, const Vertex& rhs) {
        return lhs.x < rhs.x&& lhs.y < rhs.y&& lhs.z < lhs.z;
    }
    friend inline bool operator>(const Vertex& lhs, const Vertex& rhs) {
        return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > lhs.z;
    }
    friend inline bool operator>=(const Vertex& lhs, const Vertex& rhs) {
        return lhs > rhs || lhs == rhs;
    }
    friend inline bool operator<=(const Vertex& lhs, const Vertex& rhs) {
        return lhs < rhs || lhs == rhs;
    }
    std::string to_string()
    {
        return "{x=" + std::to_string(x) + ", y=" + std::to_string(y) + ", z=" + std::to_string(z);
    }
};

struct ObjVertex {
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
};

struct Triangle {
    Vertex p1;
    Vertex p2;
    Vertex p3;
    Triangle() {
    }
    Triangle(Vertex p1, Vertex p2, Vertex p3) {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }
};

enum class Axis { ox, oy, oz };

struct Square {
    /**
     * [0] left top
     * [1] left bottom
     * [2] right bottom
     * [3] right top
     */
    Vertex vertexes[4];
    Square() {
        vertexes[0] = Vertex(0.5f, 0.5, 0.0);
        vertexes[2] = Vertex(-0.5, -0.5, 0.0);
        vertexes[1] = Vertex(vertexes[0].x, vertexes[2].y, 0.0);
        vertexes[3] = Vertex(vertexes[2].x, vertexes[0].y, 0.0);
    }
    Square(Vertex lt, Vertex rb) {
        if (lt <= rb) {
            // error
        }
        if (lt.equalX(rb)) {
            vertexes[0] = Vertex(lt.x, lt.y, lt.z);
            vertexes[1] = Vertex(lt.x, rb.y, lt.z);
            vertexes[2] = Vertex(lt.x, rb.y, rb.z);
            vertexes[3] = Vertex(lt.x, lt.y, rb.z);
        }
        else if (lt.equalY(rb)) {
            vertexes[0] = Vertex(lt.x, lt.y, lt.z);
            vertexes[1] = Vertex(rb.x, lt.y, lt.z);
            vertexes[2] = Vertex(rb.x, lt.y, rb.z);
            vertexes[3] = Vertex(lt.x, lt.y, rb.z);
        }
        else if (lt.equalZ(rb)) {
            vertexes[0] = Vertex(lt.x, lt.y, lt.z);
            vertexes[1] = Vertex(lt.x, rb.y, lt.z);
            vertexes[2] = Vertex(rb.x, rb.y, lt.z);
            vertexes[3] = Vertex(rb.x, rb.y, lt.z);
        }
        else {
            // error
        }
    }
    /*void vertexes(TwoDVertex *arr) {
        arr[0] = leftTop;
        arr[1] = TwoDVertex(leftTop.x, rightBottom.y);
        arr[2] = rightBottom;
        arr[3] = TwoDVertex(rightBottom.x, leftTop.y);
    }*/
    // Returns triangle with square vertexes in the following order: left top, left bottom, right bottom
    Triangle leftTriangle() {
        return Triangle(vertexes[0], vertexes[1], vertexes[2]);
    }
    // Returns triangle with square vertexes in the following order: left top, right bottom, right top
    Triangle rightTriangle() {
        return Triangle(vertexes[0], vertexes[2], vertexes[3]);
    }
    // Assigns two triangles to the passed array in the following order: "left triangle", "right triangle"
    void dividedSquare(Triangle* arr) {
        arr[0] = leftTriangle();
        arr[1] = rightTriangle();
    }
};



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
    Vertex vertexes[8];
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
    void standart_constructor_init(Vertex flt, Vertex brb) {
        vertexes[0] = flt;
        vertexes[1] = Vertex(flt.x, brb.y, flt.z);
        vertexes[2] = Vertex(brb.x, brb.y, flt.z);
        vertexes[3] = Vertex(brb.x, flt.y, flt.z);
        vertexes[4] = Vertex(flt.x, flt.y, brb.z);
        vertexes[5] = Vertex(flt.x, brb.y, brb.z);
        vertexes[6] = brb;
        vertexes[7] = Vertex(brb.x, flt.y, brb.z);
    }
public:
    Cube(Vertex flt, Vertex brb) {
        if (flt <= brb) {
            // ошибка
        }
        standart_constructor_init(flt, brb);
    }
    Cube(GLfloat side_len, Vertex center = Vertex(0.0, 0.0, 0.0)) {
        GLfloat half_len = side_len / 2;
        Vertex flt = Vertex(center.x - half_len, center.y + half_len, center.z + half_len);
        Vertex brb = Vertex(center.x + half_len, center.y - half_len, center.z - half_len);
        standart_constructor_init(flt, brb);
    }

    Square frontSquare() {
        return Square(vertexes[0], vertexes[2]);
    }
    Square leftSquare() {
        return Square(vertexes[4], vertexes[1]);
    }
    Square rightSquare() {
        return Square(vertexes[3], vertexes[6]);
    }
    Square backSquare() {
        return Square(vertexes[4], vertexes[6]);
    }
    Square topSquare() {
        return Square(vertexes[4], vertexes[3]);
    }
    Square bottomSquare() {
        return Square(vertexes[5], vertexes[2]);
    }
    // [0] front
    // [1] left
    // [2] right
    // [3] back
    // [4] top
    // [5] bottom
    void dividedCube(Square* arr) {
        arr[0] = frontSquare();
        arr[1] = leftSquare();
        arr[2] = rightSquare();
        arr[3] = backSquare();
        arr[4] = topSquare();
        arr[5] = bottomSquare();
    }
};