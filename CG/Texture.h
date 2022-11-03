#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include "CImg.h"

class Texture {
public:
	GLuint id;
	Texture() {

	}
	void create_rgba16f_buffer(GLuint width, GLuint height) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	void create_rgb16f_buffer(GLuint width, GLuint height) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	void create_rgba_buffer(GLuint width, GLuint height) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	void create_wrap_clamp_buffer(GLuint width, GLuint height) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	void create_wrap_clamp_rgb16f_buffer(GLuint width, GLuint height) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	void create_point_buffer(GLuint width, GLuint height) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	void create_noise_texture(GLuint width, GLuint height, const std::vector<glm::vec3>& noise) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGB, GL_FLOAT, &noise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	void bind(const GLint texture_unit = 0)
	{
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	GLuint getId() {
		return id;
	}
	void unbind()
	{
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};


class CImgTexture {
    GLuint id;
    void create_rgb16_texture(GLuint width, GLuint height, const unsigned char* data = NULL) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    void clear_texture(GLuint width, GLuint height, const unsigned char* data = NULL) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)data);
    }
    void update_texture(int x, int y, unsigned char* data) {
        glTextureSubImage2D(id, 0, x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void*)data);
        GLenum errCode;
        // Коды ошибок можно смотреть тут
        // https://www.khronos.org/opengl/wiki/OpenGL_Error
        if ((errCode = glGetError()) != GL_NO_ERROR)
            std::cout << "OpenGl error!: " << errCode << " while inserting x=" << x
            << " y=" << y << std::endl;
    }
    int _define_sign(int d) {
        if (d > 0) {
            return 1;
        }
        if (d < 0) {
            return -1;
        }
        return 0;
    }
    void checkOpenGLerror() {
        GLenum errCode;
        // Коды ошибок можно смотреть тут
        // https://www.khronos.org/opengl/wiki/OpenGL_Error
        if ((errCode = glGetError()) != GL_NO_ERROR)
            std::cout << "OpenGl error!: " << errCode << std::endl;
    }
public:
    cimg_library::CImg<unsigned char> image;
    CImgTexture(GLuint width, GLuint height) {
        this->image = cimg_library::CImg<unsigned char>(width, height, 1, 3, 255);
        create_rgb16_texture(width, height, image.data());
    }
    void clear() {
        this->image = cimg_library::CImg<unsigned char>(image.width(), image.height(), 1, 3, 255);
        clear_texture(image.width(), image.height(), image.data());
        checkOpenGLerror();
    }
    void set_rgb(glm::vec3 coord, glm::vec3 color) {
        unsigned char rgb[3];
        for (size_t i = 0; i < 3; i++) {
            image(coord.x, coord.y, 0, i) = color[i];
            rgb[i] = color[i];
        }
        update_texture(coord.x, coord.y, rgb);
    }
    void set_rgb(int x, int y, glm::vec3 color) {
        if (x > image.width() - 1 || x < 0 || y < 0 || y > image.height() - 1) {
            return;
        }
        unsigned char rgb[3];
        for (size_t i = 0; i < 3; i++) {
            image(x, y, 0, i) = color[i];
            rgb[i] = color[i];
        }
        update_texture(x, y, rgb);
    }
    void draw_st_line(glm::vec3 fc, glm::vec3 sc, glm::vec3 color) {
        unsigned char uch_color[3];
        for (size_t i = 0; i < 3; i++) {
            uch_color[i] = color[i];
        }
        image.draw_line(fc.x, fc.y, sc.x, sc.y, uch_color);
        //create_rgb16_texture(image.width(), image.height(), image.data());
    }
    void draw_line(glm::vec3 fc, glm::vec3 sc, glm::vec3 color) {
        int dx = sc.x - fc.x;
        int dy = sc.y - fc.y;

        int sign_x = _define_sign(dx);
        int sign_y = _define_sign(dy);

        dx *= sign_x;
        dy *= sign_y;
        int dl = 0;
        int dr = 0;
        int x_addr1 = 0;
        int x_addr2 = 0;
        int y_addr1 = 0;
        int y_addr2 = 0;
        float gradient;
        if (dx == 0) {
            gradient = dy;
        }
        else {
            gradient = (float)dy / (float)dx;
        }
        int last;
        if (gradient <= 1) {
            dl = dy;
            dr = dx,
                x_addr2 = sign_x;
            y_addr1 = sign_y;
            last = dx;
        }
        else {
            dl = dx;
            dr = dy;
            x_addr1 = sign_x;
            y_addr2 = sign_y;
            last = dy;
        }

        int di = 2 * dl - dr;
        int x = fc.x; int y = fc.y;
        int t = 0;

        set_rgb(x, y, color);

        while (t < last) {
            if (di < 0) {
                di += 2 * dl;
            }
            else {
                y += y_addr1;
                x += x_addr1;
                di += 2 * (dl - dr);
            }
            x += x_addr2;
            y += y_addr2;
            t += 1;
            set_rgb(x, y, color);
        }
    }
    glm::vec3 get_rgb(glm::vec3 coord) {
        glm::vec3 res;
        for (size_t i = 0; i < 3; i++) {
            res[i] = image(coord.x, coord.y, 0, i);
        }
        return res;
    }
    int get_height() {
        return image.height();
    }
    int get_width() {
        return image.width();
    }
    void* get_void_id() {
        return (void*)(intptr_t)id;
    }
};
