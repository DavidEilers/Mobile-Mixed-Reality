//
// Created by david on 02.03.21.
//

#ifndef TEAMPRAKTIKUM_OBJRENDERER_H
#define TEAMPRAKTIKUM_OBJRENDERER_H
#pragma once

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

/**
 * @class ObjRenderer
 * Manage and renders object-files
 * */
class ObjRenderer {
public:
    /**
     * @brief ObjRenderer Constructor
     * @param filename Relative path to obj-file starting at assets folder
     * @param assetMgr Android AssetManager for loading obj-file
     * */
    ObjRenderer(std::string filename, AAssetManager *assetMgr);

    ~ObjRenderer();

    /**
     * @brief Renders obj-file to framebuffer
     * */
    void draw();

    /** @brief Set the ModelMatrix*/
    void setModelMatrix(glm::mat4 model_mat);

    /** @brief Set the ViewMatrix*/
    void setViewMatrix(glm::mat4 view_mat);

    /** @brief Set the ProjectionMatrix*/
    void setProjectionMatrix(glm::mat4 projection_mat);

    /** @brief Bind VertexBuffers and set their respective vertex attribute pointer*/
    void updateVertexData();

private:
    Shader *myShader;
    GLuint programID;
    GLuint vao;
    GLuint vbo[3];
    GLuint ibo;
    GLuint mvpMatrixID;
    GLuint mvMatrixID;
    GLuint viewMatrixID;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 upvec;
    glm::vec3 lookat;
    std::vector<GLfloat> *vertices = nullptr;
    std::vector<GLfloat> *normals = nullptr;
    std::vector<GLfloat> *uvs = nullptr;
    std::vector<GLushort> *indices = nullptr;
};

#endif //TEAMPRAKTIKUM_OBJRENDERER_H
