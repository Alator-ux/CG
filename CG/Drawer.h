#pragma once
#include "OpenGLWrappers.h"
#include "Figure.h"
#include "Projection.h"

class Drawer {
    GLuint vPos;
    Shader* shader;
    OpenGLManager* manager;
    glm::mat4 ortho;
    glm::mat4 projection;
    float aspect;
    bool first;
    template <typename T>
    void prepare_for_drawing(std::vector<T>& elements, const std::string& buffer_name) {
        if (elements.size() == 0) {
            return;
        }
        shader->use_program();

        glEnableVertexAttribArray(vPos);
        glBindBuffer(GL_ARRAY_BUFFER, manager->get_buffer_id(buffer_name));
        glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
        manager->checkOpenGLerror();
    }
    void end_drawing() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(vPos);
        shader->disable_program();
        glLineWidth(1);
        manager->checkOpenGLerror();
    }
public:
    Drawer() {
    }
    Drawer(Shader* shader, const char* vPosName, GLuint w_width, GLuint w_height) {
        this->shader = shader;
        this->vPos = this->shader->get_attrib_location(vPosName);
        this->manager = OpenGLManager::get_instance();
        this->first = true;

        this->aspect = (float)w_width / (float)w_height;
        

        this->ortho = glm::ortho(0.0f, (float)w_width, (float)w_height, 0.0f,
            (float)1, -(float)1);
        set_projection_mode(projection::pers);
    }
    void set_projection_mode(projection::Type type) {
        shader->use_program();
        switch (type)
        {
        case projection::pers:
        {
            projection = projection::perspective(glm::radians(60.0f), aspect, 0.001f, 100.0f);
            shader->uniformMatrix4fv("projection", projection);
            break;
        }
        case projection::axon:
        {
            projection = projection::axonometric(PI / 6, -(PI / 3));
            shader->uniformMatrix4fv("projection", projection);
            break;
        }
        case projection::none:
        {
            projection = glm::mat4(1);
            shader->uniformMatrix4fv("projection", projection);
            break;
        }
        default:
            break;
        }
        shader->disable_program();
    }
    void draw(std::vector<Primitive>& primitives, const std::string& buffer_name) {
        prepare_for_drawing(primitives, buffer_name);
        GLint from = 0;
        GLint count = 0;
        for (Primitive& pr : primitives) {
            glLineWidth(pr.width);
            count = pr.get_points_count();
            shader->uniform4f("color", pr.color);
            glDrawArrays(pr.drawing_type, from, count);
            from += count;
        }
        end_drawing();
    }
    void draw(std::vector<Figure>& figures, const std::string& buffer_name) {
        prepare_for_drawing(figures, buffer_name);
        GLint from = 0;
        GLint count = 0;
        for (auto& fig : figures) {
            for (auto& pr : fig.faces) {
                glLineWidth(pr.width);
                count = pr.get_points_count();
                shader->uniform4f("color", pr.color);
                glDrawArrays(pr.drawing_type, from, count);
                from += count;
            }
        }
        end_drawing();
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
            //first = false;
        }
    }
    void set_vbo(const std::string& buffer_name, const std::vector<Figure>& data) {
        if (data.size() == 0) {
            return;
        }
        std::vector<glm::vec3> ndata;
        GLuint size = 0;
        for (auto& fig : data) {
            for (auto& pr : fig.faces) {
                ndata.insert(ndata.end(), pr.points.begin(), pr.points.end());
                size += GLuint(sizeof(GLfloat) * pr.points.size() * 3);
            }
        }
        if (!first) {
            manager->refresh_vbo(buffer_name, &ndata[0], size, GL_STATIC_DRAW);
        }
        else {
            manager->init_vbo(buffer_name, &ndata[0], size, GL_STATIC_DRAW);
            //first = false;
        }
    }
};