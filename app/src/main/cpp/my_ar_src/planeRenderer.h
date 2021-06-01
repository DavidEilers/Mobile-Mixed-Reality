//
// Created by david on 13.04.21.
//

#pragma once
#ifndef TEAMPRAKTIKUM_PLANERENDERER_H
#define TEAMPRAKTIKUM_PLANERENDERER_H


//#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h> //NEED GLES2Extensions for GLES3 KNOWN WORKAROUND
#include <jni.h>
#include "arcore_c_api.h"
#include <string>
#include <vector>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "glm.hpp"
#include "ext.hpp"
#include "shader.h"

//Size of Vertex Buffer Object max. 1024 triangles
const size_t VBO_SIZE = 1024*3;


/**
 * @class cameraBackground
 * Renders the Background(The Camera Image) on the framebuffer
 * */
class PlaneRenderer {
public:
    /**
     * @brief cameraBackground Constructor
     * @param assetManager Android AssetManager for loading shader-files
     * */
    PlaneRenderer(AAssetManager *assetManager);

    ~PlaneRenderer();

    /**
     * @brief Initialize needed GL Data
     * */
    void initGL();

    /**
     * @brief Renders camera image from ARCore to framebuffer
     * @param session active ARCore Session to aquire camera-frame from
     * */
    void draw();

    /**
     * @brief Compile and Link Shader prepare for usage
     */
    void initShader();


    /**
     * @brief Bind VertexBuffers and set their respective vertex attribute pointer
     * */
    void updateVertexData();

    void setVertexData(std::vector<GLfloat>* vertices_);

    /** @brief Set the ModelMatrix*/
    void setModelMatrix(glm::mat4 model_mat);

    /** @brief Set the ViewMatrix*/
    void setViewMatrix(glm::mat4 view_mat);

    /** @brief Set the ProjectionMatrix*/
    void setProjectionMatrix(glm::mat4 projection_mat);




private:
    std::vector<GLfloat>* vertices= nullptr;
    std::vector<GLushort>* indices= nullptr;
    Shader *myShader = nullptr;
    AAssetManager *assetManager;
    GLuint programID;
    GLuint vboID;
    GLuint iboID;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    GLuint mvpMatrixID;

};


#endif //TEAMPRAKTIKUM_PLANERENDERER_H
