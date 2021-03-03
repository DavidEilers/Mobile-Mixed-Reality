//
// Created by david on 02.03.21.
//

#ifndef TEAMPRAKTIKUM_SHADER_H
#define TEAMPRAKTIKUM_SHADER_H
#pragma once
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h> //NEED GLES2Extensions for GLES3 KNOWN WORKAROUND
#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

class Shader{
public:
    Shader(AAssetManager *assetMgr);
    ~Shader();
    void addShader(std::string path, GLenum shaderType);
    GLuint compileShader();

private:
    AAssetManager* assetManager;
    GLuint programID;
    struct ShaderInfo {
        GLuint shaderID;
        GLenum shaderType;
        bool linked;
        bool compiled;
    };
    ShaderInfo shaderInfos[2];
};
#endif //TEAMPRAKTIKUM_SHADER_H
