#pragma once
#include "OpenGLWrappers.h"
#include "ThreeDInterface.h"
#include "Projection.h"
#include "Camera.h"
class Drawer {
    GLuint vPos;
    Shader* shader;
    OpenGLManager* manager;
    template <typename T>
    void prepare_for_drawing(std::vector<T>& elements, const std::string& buffer_name,
        Camera& camera) {
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
        manager->checkOpenGLerror();
    }
public:
    Drawer() {
    }
    Drawer(Shader* shader, const char* vPosName, GLuint w_width, GLuint w_height) {
        this->shader = shader;
        this->vPos = this->shader->get_attrib_location(vPosName);
        this->manager = OpenGLManager::get_instance();
    }
    void draw(std::vector<primitives::Primitive>& primitives, const std::string& buffer_name,
        Camera& camera) {
        prepare_for_drawing(primitives, buffer_name, camera);
        GLint from = 0;
        GLint count = 0;
        for (primitives::Primitive& pr : primitives) {
            glLineWidth(pr.width);
            count = pr.get_points_count();
            shader->uniform4f("color", pr.colors[0]);
            glDrawArrays(pr.drawing_type, from, count);
            from += count;
        }
        end_drawing();
    }
    void set_vbo(const std::string& buffer_name, const std::vector<primitives::Primitive>& data) {
        if (data.size() == 0) {
            return;
        }
        std::vector<glm::vec3> ndata;
        GLuint size = 0;
        for (auto pr : data) {
            ndata.insert(ndata.end(), pr.points.begin(), pr.points.end());
            size += GLuint(sizeof(GLfloat) * pr.points.size() * 3);
        }
        manager->init_vbo(buffer_name, &ndata[0], size, GL_STATIC_DRAW);
    }
};