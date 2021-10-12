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


class Mesh;
class Node;

class Scene {
public:
    Scene(AAssetManager *assetManager_);
    virtual ~Scene();
    void setModel(glm::mat4 model_);
    void setView(glm::mat4 view_);
    void setProjection(glm::mat4 projection_);
    void draw();

    glm::mat4 getView();
    glm::mat4 getProjection();
    AAssetManager* getAssetManager();

    virtual void hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination)=0;
    virtual void update()=0;


protected:
    Node* getHitTestNode(glm::vec3 rayOrigin, glm::vec3 rayDestination);
    AAssetManager *assetManager=nullptr;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    Node * rootNode= nullptr;

};


#endif //TEAMPRAKTIKUM_SCENE_H
