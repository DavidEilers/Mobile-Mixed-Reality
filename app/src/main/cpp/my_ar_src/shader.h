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

/**
 * @class Shader
 * Class for loading, compiling and linking Shaders
 * */
class Shader{
public:
    /**
     * @brief Shader constructor
     * @param assetMgr Android AssetManager for loading shader-files
     * */
    Shader(AAssetManager *assetMgr);
    ~Shader();

    /**
     * @brief Add shaderCode for a Shaderstage and compile
     * @param path Relative path to shader-file starting at assets folder
     * @param shaderType Specify the shaderStage
     * */
    void addShader(std::string path, GLenum shaderType);

    /**
     * @brief Link shader
     * @return OpenGL progromID of Shader or -1 if failed
     * */
    GLuint linkShader();

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
