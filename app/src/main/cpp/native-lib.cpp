#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <jni.h>
#include <string>
#include "arcore_c_api.h"
#include <android/log.h>
#include "cameraBackground.h"
#include "objRenderer.h"
#include "Scene.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "arServer.h"


extern "C" {

ArServer* arServer= nullptr;
cameraBackground *camBack = nullptr;
Scene * scene = nullptr;
PlaneRenderer* planeRenderer = nullptr;
int displayWidth=0;
int displayHeight=0;


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
    planeRenderer = new PlaneRenderer(mgr);
    scene = new Scene(mgr);

}

JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_onDisplayGeometryChanged(
        JNIEnv *env,
        jobject activity,
        int display_rotation,
        int width,
        int height
) {
    displayWidth= width;
    displayHeight= height;
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
        if(arServer->getAnchor() == nullptr){
            arServer->createAnchorAt(x,y);
        }
    }

    __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","Will do a hitTest");
    glm::vec3 rayOrigin((2.0f*x/(float)displayWidth)-1.0f,(2.0f*y/(float)displayHeight)-1.0f,1);
    __android_log_print(ANDROID_LOG_VERBOSE, "Teampraktikum","Float x: %.2f Float y: %.2f\n DisplayHeight: %d DisplayWidth: %d\n",x,y,displayHeight,displayWidth);
    __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","RayOrigin: %0.4f %.4f %.4f",rayOrigin.x, rayOrigin.y,rayOrigin.z);
    glm::vec3 rayDirection((2.0f*x/(float)displayWidth)-1.0f,(2.0f*y/(float)displayHeight)-1.0f,-1);
    scene->hitTest(rayOrigin,rayDirection);
    __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","Did a hitTest");

}


JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_onDrawFrame(
        JNIEnv *env,
        jobject activity) {

   if(arServer->onDrawBackground(camBack->getTextureID())==true){
        camBack->draw();
   }


    glClear(GL_DEPTH_BUFFER_BIT);
    if (arServer->onDrawAnchor()==true) {
        glEnable(GL_DEPTH_TEST);
        //objRenderer->draw();
        glm::mat4 view = arServer->getViewMatrix();
        glm::mat4 projection = arServer->getProjectionMatrix();
        glm::mat4 model = arServer->getModelMatrix();
        std::string viewString="";
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                viewString.append(std::to_string(model[i][j]));
            }
            viewString.append("\n");
        }
        //__android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","ModelMatrix in nativelib:\n %s",viewString.c_str());
        //mesh->draw(model, view, projection);
        scene->setModel(model);
        scene->setView(view);
        scene->setProjection(projection);
        scene->draw();
        glDisable(GL_DEPTH_TEST);
    }

    //arServer->onDrawPlanes(planeRenderer);


}
}






