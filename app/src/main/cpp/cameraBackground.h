//
// Created by david on 14.01.21.
//
#pragma once
#ifndef TEAMPRAKTIKUM_CAMERABACKGROUND_H
#define TEAMPRAKTIKUM_CAMERABACKGROUND_H

//#include <GLES/gl.h>
#include <GLES3/gl3.h>
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



class cameraBackground {
public:
    cameraBackground(ArSession *session, ArFrame *frame, AAssetManager *assetManager);

    ~cameraBackground();

    void initGL();

    void draw(ArSession *session);

    void initShader();

    void createShader(std::string path, GLenum shaderType);

    void updateCameraFrame(ArFrame *frame);


private:
    Shader* myShader=nullptr;
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
