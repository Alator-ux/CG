#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include "OpenGLWrappers.h"



struct Color {
    glm::vec4 rgba;
    Color() {
        rgba = glm::vec4(0.0f);
    }
    Color(glm::vec4 rgba) {
        this->rgba = rgba;
    }
    Color(glm::vec3 rgb) {
        this->rgba = glm::vec4(rgb, 1.0f);
    }
    glm::vec3 to_vec3() {
        return glm::vec3(rgba.r, rgba.g, rgba.b);
    }
    static Color red() {
        return Color(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    static Color green() {
        return Color(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    static Color blue() {
        return Color(glm::vec3(0.0f, 0.0f, 1.0f));
    }
    static Color white() {
        return Color(glm::vec3(1.0f, 1.0f, 1.0f));
    }
    static Color copper() {
        return Color(glm::vec3(1.0f, 0.5f, 0.31f));
    }
    static Color dark_olive_green() {
        return Color(glm::vec3(0.33f, 0.42f, 0.18f));
    }
};

struct Primitive {
public:
    int type;
    std::vector<glm::vec3> points;
    int drawing_type;
    glm::vec3 color;
    GLint get_points_count() {
        return (GLint)points.size();
    }
    void push_point(glm::vec2 coords) {

    }
    bool primitive_is_finished() {
        return true;
    }
    static std::string get_string_name() {
        return "Primitive";
    }
};

struct Point : public Primitive {
public:
    Point(glm::vec3 coord, glm::vec3 color) {
        points.push_back(coord);
        this->color = color;
        drawing_type = GL_POINTS;
        this->type = 0;
    }
    static std::string get_string_name() {
        return "Point";
    }
};

struct Edge : public Primitive {
public:
    Edge(glm::vec3 coords, glm::vec3 color) {
        points.push_back(coords);
        this->color = color;
        drawing_type = GL_POINTS;
        this->type = 1;
    }
    void push_point(glm::vec3 coords) {
        if (points.size() == 1) {
            points.push_back(coords);
            drawing_type = GL_LINES;
        }
    }
    bool primitive_is_finished() {
        return points.size() == 2;
    }
    static std::string get_string_name() {
        return "Edge";
    }
};

struct Polygon : public Primitive {
public:
    Polygon(glm::vec3 coords, glm::vec3 color) {
        points.push_back(coords);
        this->color = color;
        drawing_type = GL_POINTS;
        this->type = 2;
    }
    void push_point(glm::vec3 coords) {
        points.push_back(coords);
        if (points.size() > 1) {
            drawing_type = GL_LINE_STRIP;
        }
    }
    bool primitive_is_finished() {
        if (points.size() < 3) {
            return false;
        }
        drawing_type = GL_LINE_LOOP;
        return true;

    }
    static std::string get_string_name() {
        return "Polygon";
    }
};

class PrimitiveFabric {
    int code = 0;
    glm::vec3 color;
    std::vector<Primitive> primitives;
    bool prim_finished = true;
    size_t max_size;
public:
    PrimitiveFabric() {
        color = glm::vec3(1.0f);
        max_size = 10;
    }
    void update_code(int code) {
        if (this->code == code) {
            return;
        }
        this->code = code;
        if (!prim_finished) {
            primitives.erase(primitives.end() - 1);
        }
        prim_finished = true;
    }
    void update_color(glm::vec3 color) {
        this->color = color;
    }
    void build(glm::vec3 coords) {
        switch (code)
        {
        case 0:
            create_point(coords);
            break;
        case 1:
            if (prim_finished) {
                create_edge(coords);
            }
            else {
                Edge* edge = reinterpret_cast<Edge*>(&primitives[primitives.size() - 1]);
                edge->push_point(coords);
                prim_finished = edge->primitive_is_finished();
            }
            break;
        case 2:
            if (prim_finished) {
                create_polygon(coords);
            }
            else {
                Polygon* polygon = reinterpret_cast<Polygon*>(&primitives[primitives.size() - 1]);
                polygon->push_point(coords);
            }
        default:
            break;
        }
    }
    void finish_primitive() {
        switch (code)
        {
        case 0:
            prim_finished = true;
            break;
        case 1:
        {
            Edge* edge = reinterpret_cast<Edge*>(&primitives[primitives.size() - 1]);
            prim_finished = edge->primitive_is_finished();
            break;
        }
        case 2: 
        {
            Polygon* polygon = reinterpret_cast<Polygon*>(&primitives[primitives.size() - 1]);
            prim_finished = polygon->primitive_is_finished();
            break;
        }
        default:
            break;
        }
    }
    void create_point(glm::vec3 coord) {
        if (primitives.size() == max_size) {
            return;
        }
        primitives.push_back(Point(coord, color));
        prim_finished = true;
    }
    void create_edge(glm::vec3 coords) {
        if (primitives.size() == max_size) {
            return;
        }
        primitives.push_back(Edge(coords, color));
        prim_finished = false;
    }
    void create_polygon(glm::vec3 coords) {
        if (primitives.size() == max_size) {
            return;
        }
        primitives.push_back(Polygon(coords, color));
        prim_finished = false;
    }
    void clear() {
        primitives.clear();
        prim_finished = true;
    }
    std::vector<Primitive>& get_items() {
        return primitives;
    }
    size_t size() {
        return primitives.size();
    }
    size_t get_max_size() {
        return max_size;
    }
};

class Drawer {
    GLuint vPos;
    Shader* shader;
    OpenGLManager* manager;
    glm::mat4 projection;
    bool first;
public:
    Drawer(){
    }
    Drawer(Shader* shader, const char* vPosName, GLuint w_width, GLuint w_height) {
        this->shader = shader;
        this->vPos = this->shader->get_attrib_location(vPosName);
        this->manager = OpenGLManager::get_instance();
        this->first = true;

        this->projection = glm::ortho(0.0f, (float)w_width, (float)w_height, 0.0f, 1.0f, -1.0f);
    }
    void draw(std::vector<Primitive>& primitives, const std::string& buffer_name) {
        GLint from = 0;
        GLint count = 0;
        shader->use_program();
        shader->uniformMatrix4fv("projection", projection);
        glEnableVertexAttribArray(vPos);
        glBindBuffer(GL_ARRAY_BUFFER, manager->get_buffer_id(buffer_name));
        glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
        manager->checkOpenGLerror();
        for (Primitive& pr : primitives) {
            count = pr.get_points_count();
            shader->uniform4f("color", pr.color);
            glDrawArrays(pr.drawing_type, from, count);
            from += count;
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(vPos);
        shader->disable_program();
        manager->checkOpenGLerror();
    }
    void set_vbo(const std::string& buffer_name, const std::vector<Primitive>& data) {
        if (data.size() == 0) {
            return;
        }
        std::vector<glm::vec3> ndata;
        GLuint size = 0;
        for (auto pr : data) {
            ndata.insert(ndata.end(), pr.points.begin(), pr.points.end());
            size += GLuint(sizeof(GLfloat) * pr.points.size() * 3);
        }
        if (!first) {
            manager->refresh_vbo(buffer_name, &ndata[0], size, GL_STATIC_DRAW);
        }
        else {
            manager->init_vbo(buffer_name, &ndata[0], size, GL_STATIC_DRAW);
            first = false;
        }
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