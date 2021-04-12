#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <jni.h>
#include <string>
#include "arcore_c_api.h"
#include <android/log.h>
#include "cameraBackground.h"
#include "objRenderer.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "arServer.h"


extern "C" {

ArServer* arServer= nullptr;
cameraBackground *camBack = nullptr;
ObjRenderer *objRenderer = nullptr;


JNIEXPORT int JNICALL
Java_com_example_teampraktikum_MainActivity_nativeOnResume(
        JNIEnv *env,
        jobject activity,
        jobject context){
    __android_log_print(ANDROID_LOG_VERBOSE, "Teampraktikum", "In nativeOnResume");


    if(arServer==nullptr){
        arServer = new ArServer();
    }

    if(arServer->onResume(env,activity,context)==false){
        __android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikum","ArServer on Resume failed");
        return -1;
    }


    if (glGetError() != GL_NO_ERROR) {
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum", "GL ERROR %d", glGetError());
    } else {
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum", "NO GL ERROR");
    }
    __android_log_print(ANDROID_LOG_VERBOSE, "Teampraktikum", "GL Extensions:\n%s",
                        glGetString(GL_EXTENSIONS));
    return 0;
}

JNIEXPORT void JNICALL Java_com_example_teampraktikum_MainActivity_nativeOnSurfaceCreated(
        JNIEnv *env,
        jobject activity,
        jobject assetManager
) {
    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);
    camBack = new cameraBackground(mgr);
    objRenderer = new ObjRenderer("obj/cube.obj", mgr);
}

JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_onDisplayGeometryChanged(
        JNIEnv *env,
        jobject activity,
        int display_rotation,
        int width,
        int height
) {
    glViewport(0, 0, width, height);
    if(arServer!= nullptr){
        arServer->setDisplayGeometry(display_rotation,width,height);
    }
}



//TODO x and y positions from touch event are not relative to the camera canvas
JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_nativeOnTouched(
        JNIEnv *env,
        jobject activity,
        float x,
        float y) {
    if(arServer!= nullptr){
        arServer->createAnchorAt(x,y);
    }

}


JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_onDrawFrame(
        JNIEnv *env,
        jobject activity) {

   if(arServer->onDrawBackground(camBack->getTextureID())==true){
        camBack->draw();
   }

    if (arServer->onDrawAnchor()==true) {
        objRenderer->setProjectionMatrix(arServer->getProjectionMatrix());
        objRenderer->setViewMatrix(arServer->getViewMatrix());
        objRenderer->setModelMatrix(arServer->getModelMatrix());
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        objRenderer->draw();
        glDisable(GL_DEPTH_TEST);
    }


}
}






