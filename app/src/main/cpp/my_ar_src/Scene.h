//
// Created by david on 17.04.21.
//

#ifndef TEAMPRAKTIKUM_SCENE_H
#define TEAMPRAKTIKUM_SCENE_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h> //NEED GLES2Extensions for GLES3 KNOWN WORKAROUND
#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "glm.hpp"
#include "ext.hpp"
#include "shader.h"
#include "objLoader.h"
#include "Mesh.h"
#include "Node.h"
#include "ext.hpp"

#include "TicTacToe.hpp"

extern class Mesh;
extern class Node;

class Scene {
public:
    Scene(AAssetManager *assetManager_,std::string ip);
    virtual ~Scene();
    void setModel(glm::mat4 model_);
    void setView(glm::mat4 view_);
    void setProjection(glm::mat4 projection_);
    void draw();
    void hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination);
    glm::mat4 getView();
    glm::mat4 getProjection();
    AAssetManager* getAssetManager();

private:

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    Node * rootNode= nullptr;
    AAssetManager *assetManager=nullptr;
    Node * fields[9];
    Mesh * crossMesh;
    Mesh * circleMesh;
    TTTSlave slave;

};


#endif //TEAMPRAKTIKUM_SCENE_H
