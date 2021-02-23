//
// Created by david on 14.01.21.
//

#include "cameraBackground.h"

cameraBackground::cameraBackground(ArSession *session, ArFrame *frame, AAssetManager *mgr) {
    this->arSess = session;
    this->arFrame = frame;
    this->assetManager = mgr;
    initGL();
}

cameraBackground::~cameraBackground() {

}

void cameraBackground::initGL() {

    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "BeforeGenTextures");
    glGenTextures(1, &imageID);
    ArImage **img = nullptr;
    //ArFrame_acquireCameraImage(arSess,arFrame,img);

    //ArImage_getWidth(arSess, reinterpret_cast<const ArImage *>(img), &width);
    //ArImage_getHeight(arSess, reinterpret_cast<const ArImage *>(img), &height);
    //glBindBuffer(GL_ARRAY_BUFFER,imageID);
    //glBufferData(GL_ARRAY_BUFFER,size_of(GLfloat)*4*width*height,img,GL_STATIC_DRAW);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "BeforeInitShader");
    initShader();
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "AfterInitShader");
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, screenQuad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLfloat *) 0 + 2);
    glEnableVertexAttribArray(1);

}


void cameraBackground::createShader(std::string path, GLenum shaderType) {
    Shader shaderData = {0, shaderType, false, false};
    //std::ifstream shaderFile("shader/screenQuadTexture.v.glsl",std::ifstream::in);
    //std::string shaderStr("");
    //if(shaderFile.is_open()){
    //    shaderStr.assign((std::istreambuf_iterator<char>(shaderFile)),std::istreambuf_iterator<char>());
    //}
    AAsset *file = AAssetManager_open(assetManager, path.c_str(), AASSET_MODE_BUFFER);
    const char *shaderChars = (char *) AAsset_getBuffer(file);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "%s", shaderChars);
    shaderData.shaderID = glCreateShader(shaderData.shaderType);
    //const char * shaderChars = shaderStr.c_str();
    glShaderSource(shaderData.shaderID, 1, &shaderChars, NULL);
    glCompileShader(shaderData.shaderID);
    AAsset_close(file);
    //shaderFile.close();

    //ERROR LOG OUTPUT Shader
    int compiled;
    glGetShaderiv(shaderData.shaderID, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        int log_length;
        glGetShaderiv(shaderData.shaderID, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> vecChar(log_length + 1);
        glGetShaderInfoLog(shaderData.shaderID, log_length, NULL, vecChar.data());
        std::string logStr(begin(vecChar), end(vecChar));
        //std::cerr << logStr<<std::endl;
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "%s", logStr.c_str());
    } else {
        shaderData.compiled = true;

        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "Shader compiled");
    }
    int pos = (shaderType == GL_FRAGMENT_SHADER) ? 0 : 1;
    shaderparts[pos] = shaderData;
}

void cameraBackground::initShader() {

    createShader("shader/screenQuadTexture.f.glsl", GL_FRAGMENT_SHADER);
    createShader("shader/screenQuadTexture.v.glsl", GL_VERTEX_SHADER);
    programID = glCreateProgram();
    glAttachShader(programID, shaderparts[0].shaderID);
    glAttachShader(programID, shaderparts[1].shaderID);
    glLinkProgram(programID);
    GLint linked;
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "Shader couldn't be compiled");
        return;
    }
    glActiveTexture(GL_TEXTURE0);
    updateCameraFrame(this->arFrame);

}

void cameraBackground::updateCameraFrame(const ArFrame *frame) {
    /*
    ArImage** img = nullptr;
    ArFrame_acquireCameraImage(arSess,arFrame,img);
    glBindTexture(GL_TEXTURE_2D,imageID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_IMG, GL_UNSIGNED_BYTE, img);
     */
}

void cameraBackground::draw(const ArSession *session, const ArFrame *frame) {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glRasterPos2i(0,0);
    //updateCameraFrame(frame);
    glUseProgram(programID);
    glBindTexture(GL_TEXTURE_2D, imageID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLfloat *) 0 + 2);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glDrawArrays(GL_STATIC_DRAW,)
    //glBindBuffer(imageID);
    //glDrawPixels(width,height,GL_RGBA,GL_FLOAT,0);
}
