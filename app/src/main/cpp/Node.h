//
// Created by david on 17.04.21.
//

#ifndef TEAMPRAKTIKUM_NODE_H
#define TEAMPRAKTIKUM_NODE_H

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
#include "Mesh.h"
#include "objLoader.h"
#include "../../../libs/third_party/glm/ext.hpp"

extern class Mesh;

class Node {
public:
    Node();
    virtual ~Node();
    void setModel(glm::mat4 model_);
    void addChild(Node node);
    void draw(glm::mat4 parentTransform);
    void setMesh(Mesh *mesh);

private:
    glm::mat4 model;
    std::vector<Node>* childs;
    Mesh* mesh;

};



#endif //TEAMPRAKTIKUM_NODE_H
