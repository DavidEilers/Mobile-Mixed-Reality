//
// Created by david on 17.04.21.
//

#ifndef TEAMPRAKTIKUM_MESH_H
#define TEAMPRAKTIKUM_MESH_H

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
#include "Scene.h"
#include "../../../libs/third_party/glm/ext.hpp"

extern class Scene;

class Mesh {
public:
    Mesh(std::string shaderName, std::string objectName, Scene *scene_);
    virtual ~Mesh();
    void draw(glm::mat4 model);
    void updateVertexData();
private:
    GLuint vboID[3];
    Shader *myShader;
    GLuint programID;
    GLuint ibo;
    GLuint mvpMatrixID;
    GLuint mvMatrixID;
    GLuint viewMatrixID;
    Scene* scene;
    size_t indicesSize;
};


#endif //TEAMPRAKTIKUM_MESH_H
