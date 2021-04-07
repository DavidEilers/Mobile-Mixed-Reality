//
// Created by david on 14.01.21.
//
#pragma once
#ifndef TEAMPRAKTIKUM_CAMERABACKGROUND_H
#define TEAMPRAKTIKUM_CAMERABACKGROUND_H

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
#include "shader.h"

static float screenQuad[24] = {
        //    x     y    u    v
        -1.0f, -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 0.0f};

static float screenQuadTex[8] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f};


//static final char* vertexShader= "shader/screenQuadTexture.v.glsl";

//static final char* fragmentShader="shader/screenQuadTexture.v.glsl";


/**
 * @class cameraBackground
 * Renders the Background(The Camera Image) on the framebuffer
 * */
class cameraBackground {
public:
    /**
     * @brief cameraBackground Constructor
     * @param assetManager Android AssetManager for loading shader-files
     * */
    cameraBackground(AAssetManager *assetManager);

    ~cameraBackground();

    /**
     * @brief Initialize needed GL Data
     * */
    void initGL();

    /**
     * @brief Renders camera image from ARCore to framebuffer
     * @param session active ARCore Session to aquire camera-frame from
     * */
    void draw(ArSession *session);

    /**
     * @brief Compile and Link Shader prepare for usage
     */
    void initShader();

    /**
     * @brief Update Texture to newest camera-frame
     * */
    void updateCameraFrame(ArFrame *frame);

    /**
     * @brief Bind VertexBuffers and set their respective vertex attribute pointer
     * */
    void updateVertexData();


private:
    Shader *myShader = nullptr;
    AAssetManager *assetManager;
    GLuint programID;
    GLuint vao;
    GLuint vboID;
    GLuint imageID;
    GLuint samplerLoc;
    ArSession *arSess = nullptr;
    ArFrame *arFrame = nullptr;
    int width = 0;
    int height = 0;

};


#endif //TEAMPRAKTIKUM_CAMERABACKGROUND_H
