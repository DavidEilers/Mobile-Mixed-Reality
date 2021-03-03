//
// Created by david on 14.01.21.
//

#include "cameraBackground.h"
#include "shader.h"

cameraBackground::cameraBackground(ArSession *session, ArFrame *frame, AAssetManager *mgr) {
    this->arSess = session;
    this->arFrame = frame;
    this->assetManager = mgr;
    initGL();
}

cameraBackground::~cameraBackground() {

}

void cameraBackground::initGL() {
    glGenVertexArrays(1,&vao);
    glEnableVertexAttribArray(vao);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "BeforeGenTextures");
    glGenTextures(1, &imageID);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES,imageID);
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
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLfloat *) 0 + 2);
    glEnableVertexAttribArray(1);
    glDisableVertexAttribArray(vao);

}




void cameraBackground::initShader() {

    this->myShader = new Shader(assetManager);
    myShader->addShader("shader/screenQuadTexture.f.glsl", GL_FRAGMENT_SHADER);
    myShader->addShader("shader/screenQuadTexture.v.glsl", GL_VERTEX_SHADER);
    programID= myShader->compileShader();
    glActiveTexture(GL_TEXTURE0);

}

void cameraBackground::updateCameraFrame(ArFrame *frame) {

    glBindTexture(GL_TEXTURE_EXTERNAL_OES,imageID);
    ArSession_setCameraTextureName(arSess,imageID);
    if (ArSession_update(arSess, frame) != AR_SUCCESS) {
        __android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikum","ArSession_update error");
    }


}

void cameraBackground::draw(ArSession *session) {
    glEnableVertexAttribArray(vao);
    this->arSess= session;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if(arSess!= nullptr) {
        ArFrame *frame;
        ArFrame_create(session, &frame);
        updateCameraFrame(frame);

        glUseProgram(programID);
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, imageID);

        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLfloat *) 0 + 2);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        ArFrame_destroy(frame);
    }
    glDisableVertexAttribArray(vao);
}
