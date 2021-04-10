//
// Created by david on 14.01.21.
//

#include "cameraBackground.h"
#include "shader.h"

cameraBackground::cameraBackground(AAssetManager *mgr) {
    this->arSess = nullptr; //unknown on creation
    this->assetManager = mgr;
    initGL();
}

cameraBackground::~cameraBackground() {

}

void cameraBackground::initGL() {
//    glGenVertexArraysOES(1,&vao);
//    glBindVertexArray(vao);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "BeforeGenTextures");
    glGenTextures(1, &imageID);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, imageID);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLfloat *) 0 + 2);
    glEnableVertexAttribArray(1);
//    glBindVertexArray(-1);

}


void cameraBackground::initShader() {

    this->myShader = new Shader(assetManager);
    myShader->addShader("shader/screenQuadTexture.f.glsl", GL_FRAGMENT_SHADER);
    myShader->addShader("shader/screenQuadTexture.v.glsl", GL_VERTEX_SHADER);
    programID = myShader->linkShader();
    samplerLoc = glGetUniformLocation(programID, "image");
    glUseProgram(programID);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(samplerLoc, 0);
    //glUseProgram(-1);

}

GLuint cameraBackground::getTextureID() {
    return imageID;
}

void cameraBackground::updateCameraFrame(ArFrame *frame) {

    glBindTexture(GL_TEXTURE_EXTERNAL_OES, imageID);
    ArSession_setCameraTextureName(arSess, imageID);
    if (ArSession_update(arSess, frame) != AR_SUCCESS) {
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum", "ArSession_update error");
    }


}

void cameraBackground::updateVertexData() {
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLfloat *) 0 + 2);
    glEnableVertexAttribArray(1);
}

void cameraBackground::draw(ArSession *session) {
    //glBindVertexArray(vao);
    this->arSess = session;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (arSess != nullptr) {
//        ArFrame *frame;
//        ArFrame_create(session, &frame);
//        updateCameraFrame(frame);
        if (imageID == -1) { return; }
        glUseProgram(programID);
        updateVertexData();
        glUniform1i(samplerLoc, 0);
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, imageID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
     //   ArFrame_destroy(frame);
    }
//    glBindVertexArray(-1);
}
