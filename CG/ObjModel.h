#pragma once
#include "Texture.h"
#include "ObjMesh.h"

class Model {
protected:
    ObjTexture* overrideTextureDiffuse;
    ObjTexture* overrideTextureSpecular;
    OpenGLManager* manager;
    std::vector<Mesh> meshes;
    bool hasTexture = false;
public:
    float radius = 0.f;
    glm::vec3 center;
    glm::mat4 mm = glm::mat4(1.f);

    Material material;
    Model() {

    }
    Model(const char* objFile) {
        std::vector<ObjVertex> mesh = loadOBJ(objFile, center);
        radius = glm::distance(glm::vec3(0.f), center);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }
    Model(
        const char* objFile,
        Material mat
    ) {
        manager = OpenGLManager::get_instance();
        material = mat;
        hasTexture = true;
        center = glm::vec3(0.f);
        std::vector<ObjVertex> mesh = loadOBJ(objFile, center);
        radius = glm::distance(glm::vec3(0.f), center);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }
    Model(
        const char* objFile,
        ObjTexture tex
    ) {
        manager = OpenGLManager::get_instance();
        material = Material(tex);
        hasTexture = true;
        std::vector<ObjVertex> mesh = loadOBJ(objFile, center);
        radius = glm::distance(glm::vec3(0.f), center);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }
    void render(size_t count = 1, unsigned char mode = GL_TRIANGLES)
    {
        manager->checkOpenGLerror();
        //Draw
        for (auto& i : this->meshes) {
            if (hasTexture) {
                material.texture.bind(0);
            }
            manager->checkOpenGLerror();
            i.render(count, mode); //Activates shader also
            /*if (hasTexture) {
                glActiveTexture(0);
                GLSL::checkOpenGLerror();
            }*/
        }
    }
};