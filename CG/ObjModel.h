#pragma once
#include "Texture.h"
#include "ObjMesh.h"

class Model {
private:
    ObjTexture* overrideTextureDiffuse;
    ObjTexture* overrideTextureSpecular;
    ObjTexture texture;
    OpenGLManager* manager;
    std::vector<Mesh> meshes;
    bool hasTexture = false;
public:
    Model() {

    }
    Model(const char* objFile) {
        std::vector<ObjVertex> mesh = loadOBJ(objFile);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }
    Model(
        const char* objFile,
        ObjTexture tex
    ) {
        manager = OpenGLManager::get_instance();
        texture = tex;
        manager->checkOpenGLerror();
        hasTexture = true;
        std::vector<ObjVertex> mesh = loadOBJ(objFile);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }
    Model(
        const char* objFile,
        const char* textureFile,
        const char flipTexture = 'n'
    ) {
        manager = OpenGLManager::get_instance();
        texture = ObjTexture(textureFile, flipTexture);
        manager->checkOpenGLerror();
        hasTexture = true;
        std::vector<ObjVertex> mesh = loadOBJ(objFile);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }

    void render(size_t count = 1)
    {
        manager->checkOpenGLerror();

        //Draw
        for (auto& i : this->meshes) {
            if (hasTexture) {
                texture.bind(0);
            }
            manager->checkOpenGLerror();
            i.render(count); //Activates shader also
            /*if (hasTexture) {
                glActiveTexture(0);
                GLSL::checkOpenGLerror();
            }*/
        }
    }
};