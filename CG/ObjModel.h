#pragma once
#include "Texture.h"
#include "ObjMesh.h"

class Model {
private:
    ObjTexture* overrideTextureDiffuse;
    ObjTexture* overrideTextureSpecular;
    OpenGLManager* manager;
    std::vector<Mesh> meshes;
    bool hasTexture = false;
public:
    glm::mat4 model_matrix = glm::mat4(1.f);
    glm::mat4 normal_matrix = glm::mat4(1.f);
    Material material;
    Model() {

    }

    Model(const char* objFile) {
        std::vector<ObjVertex> mesh = loadOBJ(objFile);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }
    /*Model(const Model& other) {
        this->overrideTextureDiffuse = other.overrideTextureDiffuse;
        this->overrideTextureSpecular = other.overrideTextureSpecular;
        this->manager = other.manager;
        this->hasTexture = other.hasTexture;
        this->meshes = std::vector<Mesh>();
        this->meshes.insert(this->meshes.end(), other.meshes.begin(), other.meshes.end());
        this->material = other.material;
    }*/
    Model(
        const char* objFile,
        Material mat
    ) {
        manager = OpenGLManager::get_instance();
        material = mat;
        manager->checkOpenGLerror();
        hasTexture = true;
        std::vector<ObjVertex> mesh = loadOBJ(objFile);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }
    Model(
        const char* objFile,
        ObjTexture tex
    ) {
        manager = OpenGLManager::get_instance();
        material = Material(tex);
        manager->checkOpenGLerror();
        hasTexture = true;
        std::vector<ObjVertex> mesh = loadOBJ(objFile);
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