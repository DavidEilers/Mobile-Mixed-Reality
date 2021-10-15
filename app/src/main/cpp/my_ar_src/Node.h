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
#include "BoundingBox.h"
#include "ext.hpp"

class Mesh;
class Scene;

class Node {
public:
    Node(Scene* scene_);
    virtual ~Node();
    void setModel(glm::mat4 model_);
    void addChild(Node* node);
    size_t getChildCount();
    void draw(glm::mat4 parentTransform);
    void setMesh(Mesh *mesh);
    void setBoundingBox(glm::vec3 *box);
    void prepareBoundingBoxShader();
    void renderBoundingBox(glm::mat4 model);
    void clearChilds();
    Node* hitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection,glm::mat4 parentTransform, glm::mat4 view);
    void setColor(glm::vec4 color_);

private:
    glm::vec4 color;
    GLuint programID;
    GLuint mvpID;
    GLuint vboID;
    GLuint iboID;
    Scene *scene=nullptr;
    Shader* boundingBoxShader=nullptr;
    bool wasHit=false;
    glm::vec3 *box=nullptr;
    BoundingBox * boundingBox=nullptr;
    glm::mat4 model;
    std::vector<Node*>* childs;
    Mesh* mesh=nullptr;

};



#endif //TEAMPRAKTIKUM_NODE_H
