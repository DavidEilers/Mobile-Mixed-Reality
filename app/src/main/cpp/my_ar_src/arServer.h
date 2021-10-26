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
 * A Class which manages ARCore and a single Anchor
 * */
class ArServer{
public:
    /**
     * @brief ArServer constructor checks for Installed ArCore
     */
    ArServer();
    ~ArServer();

    /**
     * @brief Updates the model-matrix of the anchor
     * @return Returns true if anchor was created and is trackable else false
     */
    bool onDrawAnchor();

    /**
     * @brief Updates the camera-frame of ArCore
     * @param textureID OpenGL texture (GL_TEXTURE_EXTERNAL_OES) which the camera-frame gets bind to
     * @return Returns true if camera-frame could be updates else false
     */
    bool onDrawBackground(GLuint textureID);

    /**
     * @brief Tries to resume the ArSession
     * @param env_ JNIEnv which ARCore needs
     * @param activity_ Activity whihc ARCore needs
     * @param context_ Context which ARCore needs
     * @return Returns true if ArCore is installed and ArCoreSession could be created and resumed successfully
     */
    bool onResume(JNIEnv *env_, jobject activity_, jobject context_);

    /**
     * @brief Call if the Displaysize changes
     * @param displayRotation_ Display rotation specified by android.view.Surface constants: ROTATION_0, ROTATION_90, ROTATION_180 and ROTATION_270
     * @param width_ Width in pixels
     * @param height_ Height in pixels
     */
    void setDisplayGeometry(int displayRotation_, int width_, int height_);

    /**
     * @brief Create Anchor at position
     * @param x x position as given by Android UI Event
     * @param y y position as given by Android UI Event
     */
    void createAnchorAt(float x, float y);

    /**
     * @brief Renders detected Planes from ARCore
     * @param planeRenderer Renderer which is able to draw planes
     */
    void onDrawPlanes(PlaneRenderer* planeRenderer);

    /**
     * @brief Check if a surface was already detected
     * @return Returns true if surface was detected
     */
    bool hasDetectedSurface();

    /**
     * @brief Get the Anchor
     * @return Returns the managed anchor and nullptr if no anchor was created yet
     */
    ArAnchor * getAnchor();

    /**
     * @brief Get the model-matrix of the anchor
     * @return Returns the model-matrix as a glm::mat4
     */
    glm::mat4 getModelMatrix();

    /**
     * @brief Get the view-matrix of the ARCamera
     * @return Returns the view-matrix as a glm::mat4
     */
    glm::mat4 getViewMatrix();

    /**
     * @brief Get the projection-matrix of the ARCamera
     * @return Returns the projection-matrix as a glm::mat4
     */
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
