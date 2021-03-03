//
// Created by david on 02.03.21.
//

#ifndef TEAMPRAKTIKUM_OBJRENDERER_H
#define TEAMPRAKTIKUM_OBJRENDERER_H
#pragma once
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h> //NEED GLES2Extensions for GLES3 KNOWN WORKAROUND
#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "shader.h"
#include "objLoader.h"

class ObjRenderer{
public:
    ObjRenderer(std::string filename,AAssetManager* assetMgr);
    ~ObjRenderer();
    void draw();
private:
    Shader* myShader;
    GLuint programID;
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    std::vector<GLfloat>* vertices=nullptr;
    std::vector<GLfloat>* normals=nullptr;
    std::vector<GLfloat>* uvs=nullptr;
    std::vector<GLushort>* indices=nullptr;
};
#endif //TEAMPRAKTIKUM_OBJRENDERER_H
