//
// Created by david on 02.03.21.
//
#include "shader.h"


Shader::Shader(AAssetManager *assetMgr){
    this->assetManager = assetMgr;
}
Shader::~Shader(){}

void Shader::addShader(std::string path, GLenum shaderType){
    ShaderInfo shaderInfo = {0, shaderType, false, false};
    AAsset *file = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);
    size_t size = AAsset_getLength(file);
    char *shaderChars = (char*) malloc (sizeof(char)*size+1);
    AAsset_read(file,shaderChars,size);
    shaderChars[size]='\0';
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "%s", shaderChars);
    shaderInfo.shaderID = glCreateShader(shaderInfo.shaderType);

    glShaderSource(shaderInfo.shaderID, 1, &shaderChars, NULL);
    glCompileShader(shaderInfo.shaderID);
    AAsset_close(file);

    //ERROR LOG OUTPUT Shader
    int compiled;
    glGetShaderiv(shaderInfo.shaderID, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        int log_length;
        glGetShaderiv(shaderInfo.shaderID, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> vecChar(log_length + 1);
        glGetShaderInfoLog(shaderInfo.shaderID, log_length, NULL, vecChar.data());
        std::string logStr(begin(vecChar), end(vecChar));
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "%s", logStr.c_str());
    } else {
        shaderInfo.compiled = true;

        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "Shader compiled");
    }
    int pos = (shaderType == GL_FRAGMENT_SHADER) ? 0 : 1;
    shaderInfos[pos] = shaderInfo;

}

GLuint Shader::compileShader(){
    this->programID = glCreateProgram();
    glAttachShader(programID, shaderInfos[0].shaderID);
    glAttachShader(programID, shaderInfos[1].shaderID);
    glLinkProgram(programID);
    GLint linked;
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "Shader couldn't be compiled");
        return -1;
    }else{
        return programID;
    }
}
