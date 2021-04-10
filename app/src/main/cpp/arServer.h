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


/**
 * @class ArServer
 * A Class which manages ARCore
 * */
class ArServer{
public:
    ArServer();
    ~ArServer();
    void onDrawFrame();
    void checkInstall();
private:
    ArSession *arSession;
    ArFrame *arFrame;
    bool installRequested;
    GLuint imageID = -1;


};
#endif //TEAMPRAKTIKUM_ARSERVER_H
