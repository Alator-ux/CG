#pragma once
#include "OpenGLWrappers.h"
#include "ThreeDInterface.h"
#include "Projection.h"
#include "Camera.h"
class Drawer {
    GLuint vPos;
    Shader* shader;
    OpenGLManager* manager;
    glm::mat4 ortho;
    glm::mat4 projection;
    float aspect;
    bool first;
    template <typename T>
    void prepare_for_drawing(std::vector<T>& elements, const std::string& buffer_name,
        Camera& camera) {
        if (elements.size() == 0) {
            return;
        }
        auto view = camera.GetViewMatrix();
        shader->use_program();
        shader->uniformMatrix4fv("view", view);
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
        

        this->ortho = glm::ortho(0.0f, (float)w_width, 0.0f, (float)w_height,
            -(float)w_width, (float)w_width);
        set_projection_mode(projection::pers);
    }
    void set_projection_mode(projection::Type type) {
        shader->use_program();
        switch (type)
        {
        case projection::pers:
        {
            projection = projection::perspective(glm::radians(60.0f), aspect, 0.001f, 500.0f);
            shader->uniformMatrix4fv("projection", projection);
            break;
        }
        case projection::axon:
        {
            projection = projection::axonometric(PI / 6, -(PI / 3));
            projection = ortho;
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
    void draw(std::vector<primitives::Primitive>& primitives, const std::string& buffer_name,
        Camera& camera) {
        prepare_for_drawing(primitives, buffer_name, camera);
        GLint from = 0;
        GLint count = 0;
        for (primitives::Primitive& pr : primitives) {
            glLineWidth(pr.width);
            count = pr.get_points_count();
            shader->uniform4f("color", pr.color);
            glDrawArrays(pr.drawing_type, from, count);
            from += count;
        }
        end_drawing();
    }
    void draw(std::vector<HighLevelInterface>& highlvl_objs, const std::string& buffer_name,
        Camera& camera) {
        prepare_for_drawing(highlvl_objs, buffer_name, camera);
        GLint from = 0;
        GLint count = 0;
        for (auto& highlvl_obj : highlvl_objs) {
            for (auto& pr : highlvl_obj.objects) {
                glLineWidth(pr.width);
                count = pr.get_points_count();
                shader->uniform4f("color", pr.color);
                glDrawArrays(pr.drawing_type, from, count);
                from += count;
            }
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
        if (!first) {
            manager->refresh_vbo(buffer_name, &ndata[0], size, GL_STATIC_DRAW);
        }
        else {
            manager->init_vbo(buffer_name, &ndata[0], size, GL_STATIC_DRAW);
            //first = false;
        }
    }
    void set_vbo(const std::string& buffer_name, const std::vector<HighLevelInterface>& data) {
        if (data.size() == 0) {
            return;
        }
        std::vector<glm::vec3> ndata;
        GLuint size = 0;
        for (auto& highlvl_obj : data) {
            for (auto& pr : highlvl_obj.objects) {
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