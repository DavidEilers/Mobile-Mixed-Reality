//
// Created by david on 10.04.21.
//
#pragma once
#ifndef TEAMPRAKTIKUM_ARSERVER_H
#define TEAMPRAKTIKUM_ARSERVER_H

#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h> //NEED GLES2Extensions for GLES3 KNOWN WORKAROUND
#include "arcore_c_api.h"
#include "glm.hpp"
#include "ext.hpp"
#include "planeRenderer.h"
#include <string>
#include <sys/time.h>


/**
 * @class ArServer
 * A Class which manages ARCore
 * */
class ArServer{
public:
    ArServer();
    ~ArServer();
    bool onDrawAnchor();
    bool onDrawBackground(GLuint textureID);
    void checkInstall();
    bool onResume(JNIEnv *env_, jobject activity_, jobject context_);
    void setDisplayGeometry(int displayRotation_, int width_, int height_);
    void createAnchorAt(float x, float y);
    void onDrawPlanes(PlaneRenderer* planeRenderer);
    bool hasDetectedSurface();
    ArAnchor * getAnchor();
    glm::mat4 getModelMatrix();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
private:
    void drawPlane(PlaneRenderer* renderer,ArPlane* plane);
    bool isArCoreInstalled();
    bool createArSession();
    ArSession *arSession;
    ArFrame *arFrame;
    ArAnchor *anchor;
    bool installRequested;
    int displayRotation;
    int width;
    int height;
    GLuint imageID = -1;
    JNIEnv *env;
    jobject context;
    jobject activity;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    int32_t plane_list_size = 0;
};
#endif //TEAMPRAKTIKUM_ARSERVER_H
