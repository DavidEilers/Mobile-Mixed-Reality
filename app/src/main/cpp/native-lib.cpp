#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <jni.h>
#include <string>
#include "arcore_c_api.h"
#include <android/log.h>
#include "cameraBackground.h"
#include "objRenderer.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


extern "C" {


JNIEXPORT jstring JNICALL
Java_com_example_teampraktikum_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject activity/* this */, jobject context) {
    std::string errorStr = "ERROR";
    std::string successStr = "Success";
    //nativeOnCreate();
    //__android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum", "The value is %d", test());
    return env->NewStringUTF("Hello World!");
}

bool install_requested_ = false;
ArSession *ar_session_ = nullptr;
ArFrame *ar_frame_ = nullptr;
int width_ = 1;
int height_ = 1;
int display_rotation_ = 0;
cameraBackground* camBack= nullptr;
ObjRenderer* objRenderer = nullptr;
int canvasX_offset=0;
int canvasY_offset=0;
ArAnchor * anchor= nullptr;

JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_nativeOnResume(
        JNIEnv *env,
        jobject activity,
        jobject context) {
    if (ar_session_ == nullptr) {
        bool user_requested_install = !install_requested_;

        ArInstallStatus install_status;
        // Ensure Google Play Services for AR and ARCore device profile data are
        // installed and up to date.
        ArStatus error = ArCoreApk_requestInstall(
                env, activity, user_requested_install, &install_status);
        if (error != AR_SUCCESS) {
            // Inform user of error.
            return;
        }

        switch (install_status) {
            case AR_INSTALL_STATUS_INSTALLED:
                break;
            case AR_INSTALL_STATUS_INSTALL_REQUESTED:
                // When this method returns `AR_INSTALL_STATUS_INSTALL_REQUESTED`:
                // 1. This activity will be paused.
                // 2. The user is prompted to install or update Google Play
                //    Services for AR (market://details?id=com.google.ar.core).
                // 3. ARCore downloads the latest device profile data.
                // 4. This activity is resumed. The next invocation of
                //    ArCoreApk_requestInstall will either return
                //    `AR_INSTALL_STATUS_INSTALLED` or throw an exception if the
                //    installation or update did not succeed.
                install_requested_ = true;
                return;
        }

        // Request camera permissions.

        error = ArSession_create(env, context, &ar_session_);
        if (error != AR_SUCCESS) {
            // Inform user of error.
            __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                                "ArSession couldn't be created ArStatusCode: %d", error);
            return;
        }


        // Configure session
        ArConfig *ar_config = nullptr;
        ArConfig_create(ar_session_, &ar_config);
        ArConfig_setDepthMode(ar_session_, ar_config, AR_DEPTH_MODE_DISABLED);
        ArConfig_setInstantPlacementMode(ar_session_, ar_config,
                                         AR_INSTANT_PLACEMENT_MODE_LOCAL_Y_UP);
        if (ArSession_configure(ar_session_, ar_config) != AR_SUCCESS) {
            return;
        }
        ArConfig_destroy(ar_config);

        ArFrame_create(ar_session_, &ar_frame_);
        ArSession_setDisplayGeometry(ar_session_, display_rotation_, width_, height_);
        ArFrame_destroy(ar_frame_);
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "Created AR Session successfully");
    }

    // Normal onResume behavior
    const ArStatus status = ArSession_resume(ar_session_);
    if (status != AR_SUCCESS) {
        return;
    }
   if(glGetError()!=GL_NO_ERROR) {
       __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum", "GL ERROR %d",glGetError());
   }else{
       __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum", "NO GL ERROR");
   }
}

JNIEXPORT void JNICALL Java_com_example_teampraktikum_MainActivity_nativeOnSurfaceCreated(
        JNIEnv* env,
        jobject activity,
        jobject assetManager
        ) {
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    camBack = new cameraBackground(nullptr, nullptr,mgr);
    objRenderer = new ObjRenderer("obj/cube.obj",mgr);
}

JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_onDisplayGeometryChanged(
        JNIEnv *env,
        jobject activity,
        int display_rotation,
        int width,
        int height
) {
    display_rotation_ = display_rotation;
    width_ = width;
    height_ = height;
    glViewport(0, 0, width, height);
    ArSession_setDisplayGeometry(ar_session_, display_rotation_, width_, height_);
}


JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_nativeSetCanvasOffset(
        JNIEnv *env,
        jint x,
        jint y) {
    __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","Surface starts at x:%d y:%d",x,y);


}

//TODO x and y positions from touch event are not relative to the camera canvas
JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_nativeOnTouched(
        JNIEnv *env,
        float x,
        float y) {
    __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","Width %d  Height %d",width_,height_);
    __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","Touched detected on x:%f y:%f",x,y);
    __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","RelativePosition on x:%f y:%f",x/width_,y/height_);


    ArFrame *frame;
    ArFrame_create(ar_session_, &frame);

    if(ArSession_update(ar_session_, frame) == AR_SUCCESS) {
        ArHitResultList *hit_result_list = nullptr;
        ArHitResultList_create(ar_session_, &hit_result_list);
        ArFrame_hitTestInstantPlacement(ar_session_, frame, x, y,
                                        20.0f,
                                        hit_result_list);
        int32_t hit_result_list_size = 0;
        ArHitResultList_getSize(ar_session_, hit_result_list,
                                &hit_result_list_size);

        if(hit_result_list_size>0) {
            ArHitResult *ar_hit = nullptr;
            ArHitResult_create(ar_session_, &ar_hit);
            ArHitResultList_getItem(ar_session_, hit_result_list, 0, ar_hit);

            ArTrackable *ar_trackable = nullptr;
            ArHitResult_acquireTrackable(ar_session_, ar_hit, &ar_trackable);
            ArTrackableType ar_trackable_type = AR_TRACKABLE_NOT_VALID;
            ArTrackable_getType(ar_session_, ar_trackable, &ar_trackable_type);

            std::string type;
            switch (ar_trackable_type) {
                case AR_TRACKABLE_POINT:
                    type = "point";
                    break;
                case AR_TRACKABLE_PLANE:
                    type = "plane";
                    break;
                case AR_TRACKABLE_INSTANT_PLACEMENT_POINT:
                    type = "instant-placement-point";
                    break;
                default:
                    type = "unknown";
                    break;
            }

            if (anchor != nullptr) {
                ArAnchor_release(anchor);
            }
            ArHitResult_acquireNewAnchor(ar_session_, ar_hit, &anchor);
            __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","Anchor %s created successfully",type.c_str());
        }
            ArHitResultList_destroy(hit_result_list);


    }

    ArFrame_destroy(frame);

}


JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_onDrawFrame(
        JNIEnv *env,
        jobject activity) {
    //glClearColor(0.7f,0.0f,0.0f,1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    ArFrame *frame;
    ArFrame_create(ar_session_, &frame);

    glm::mat4 view_mat;
    glm::mat4 projection_mat;
    glm::mat4 model_mat;
    ArCamera* ar_camera;
    ArFrame_acquireCamera(ar_session_, ar_frame_, &ar_camera);
    ArCamera_getViewMatrix(ar_session_, ar_camera, glm::value_ptr(view_mat));
    ArCamera_getProjectionMatrix(ar_session_, ar_camera,
            /*near=*/0.1f, /*far=*/100.f,
                                 glm::value_ptr(projection_mat));
    ArCamera_release(ar_camera);
    if(anchor!= nullptr) {
        ArPose *pose_;
        ArPose_create(ar_session_, nullptr, &pose_);
        ArAnchor_getPose(ar_session_, anchor, pose_);
        ArPose_getMatrix(ar_session_, pose_, glm::value_ptr(model_mat));
        ArPose_destroy(pose_);
        objRenderer->setModelMatrix(model_mat);
    }

    objRenderer->setViewMatrix(view_mat);
    objRenderer->setProjectionMatrix(projection_mat);

    // Get light estimation value.
    ArLightEstimate* ar_light_estimate;
    ArLightEstimateState ar_light_estimate_state;
    ArLightEstimate_create(ar_session_, &ar_light_estimate);

    ArFrame_getLightEstimate(ar_session_, ar_frame_, ar_light_estimate);
    ArLightEstimate_getState(ar_session_, ar_light_estimate,
                             &ar_light_estimate_state);

    // Set light intensity to default. Intensity value ranges from 0.0f to 1.0f.
    // The first three components are color scaling factors.
    // The last one is the average pixel intensity in gamma space.
    float color_correction[4] = {1.f, 1.f, 1.f, 1.f};
    if (ar_light_estimate_state == AR_LIGHT_ESTIMATE_STATE_VALID) {
        ArLightEstimate_getColorCorrection(ar_session_, ar_light_estimate,
                                           color_correction);
    }

    ArLightEstimate_destroy(ar_light_estimate);
    ar_light_estimate = nullptr;

    camBack->draw(ar_session_);

    if(anchor!= nullptr) {
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        objRenderer->draw();
        glDisable(GL_DEPTH_TEST);
    }

    ArFrame_destroy(frame);

}
}




