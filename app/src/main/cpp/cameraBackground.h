//
// Created by david on 14.01.21.
//
#pragma once
#ifndef TEAMPRAKTIKUM_CAMERABACKGROUND_H
#define TEAMPRAKTIKUM_CAMERABACKGROUND_H

#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <jni.h>
#include "arcore_c_api.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

static float screenQuad[24] = {
        //    x     y    u    v
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f};

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

    void draw(const ArSession *session, const ArFrame *frame);

    void initShader();

    void createShader(std::string path, GLenum shaderType);

    void updateCameraFrame(const ArFrame *frame);


private:
    AAssetManager *assetManager;
    GLuint programID;
    GLuint vboID;
    GLuint imageID;
    ArSession *arSess = nullptr;
    ArFrame *arFrame = nullptr;
    int width = 0;
    int height = 0;
    struct Shader {
        GLuint shaderID;
        GLenum shaderType;
        bool linked;
        bool compiled;
    };
    Shader shaderparts[2];

};


#endif //TEAMPRAKTIKUM_CAMERABACKGROUND_H
