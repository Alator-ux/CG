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
        const char* textureFile
    ) {
        manager = OpenGLManager::get_instance();
        texture = ObjTexture(textureFile);
        manager->checkOpenGLerror();
        hasTexture = true;
        std::vector<ObjVertex> mesh = loadOBJ(objFile);
        meshes.push_back(Mesh(mesh.data(), mesh.size(), NULL, 0));
    }

    void render()
    {
        manager->checkOpenGLerror();

        //Draw
        for (auto& i : this->meshes) {
            if (hasTexture) {
                texture.bind(0);
            }
            i.render(); //Activates shader also
            /*if (hasTexture) {
                glActiveTexture(0);
                GLSL::checkOpenGLerror();
            }*/
        }
    }
};