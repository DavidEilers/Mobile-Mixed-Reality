//
// Created by david on 10.04.21.
//
#include "arServer.h"

ArServer::ArServer() {
    arSession= nullptr;
    arFrame= nullptr;
    installRequested=false;
    imageID=-1;
    displayRotation=0;
    width=1;
    height=1;
    checkInstall();
}

void ArServer::checkInstall() {

}
ArServer::~ArServer() {}

bool ArServer::onDrawBackground(GLuint textureID) {
    ArSession_setCameraTextureName(arSession, textureID);
    if (ArSession_update(arSession, arFrame) != AR_SUCCESS) {
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum", "ArSession_update error");
        return false;
    }
    return true;
}

bool ArServer::onDrawAnchor() {

    ArCamera *ar_camera;
    ArFrame_acquireCamera(arSession, arFrame, &ar_camera);
    ArCamera_getViewMatrix(arSession, ar_camera, glm::value_ptr(view));
    ArCamera_getProjectionMatrix(arSession, ar_camera,
            /*near=*/0.1f, /*far=*/100.f,
                                 glm::value_ptr(projection));
    ArTrackingState trackingState;
    ArCamera_getTrackingState(arSession,ar_camera,&trackingState);
    if(trackingState!=AR_TRACKING_STATE_TRACKING){
        ArCamera_release(ar_camera);
        //__android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikum","No Trackingstate in OnDrawFrame");
        return false;
    }
    ArCamera_release(ar_camera);
    if (anchor != nullptr) {
        ArPose *pose_;
        ArPose_create(arSession, nullptr, &pose_);
        ArAnchor_getPose(arSession, anchor, pose_);
        ArPose_getMatrix(arSession, pose_, glm::value_ptr(model));
        ArPose_destroy(pose_);
        return true;
    }
    return false;

}

bool ArServer::onResume(JNIEnv *env_, jobject activity_, jobject context_) {
    this->env=env_;
    this->context= context_;
    this->activity = activity_;

    if(isArCoreInstalled()==false){
        __android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikum","ArCore not installed");
        return false;
    }

    if(arSession== nullptr){
        if(createArSession()==false){
            __android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikum","ArSession couldn't be created");
            return false;
        }
    }

    const ArStatus status = ArSession_resume(arSession);
    if (status != AR_SUCCESS) {
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "ArSession resume failed");
        return false;
    }
    return true;
}

void ArServer::setDisplayGeometry(int displayRotation_, int width_, int height_) {
    this->displayRotation=displayRotation_;
    this->width = width_;
    this->height = height_;
    if(arSession!= nullptr) {
        ArSession_setDisplayGeometry(arSession, displayRotation, width, height);
    }

}

glm::mat4 ArServer::getModelMatrix() {
    return model;
}
glm::mat4 ArServer::getProjectionMatrix() {
    return projection;
}
glm::mat4 ArServer::getViewMatrix() {
    return view;
}

bool ArServer::createArSession() {

        ArStatus error;
        error = ArSession_create(env, context, &arSession);
        if (error != AR_SUCCESS) {
            // Inform user of error.
            __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                                "ArSession couldn't be created ArStatusCode: %d", error);
            return false;
        }


        // Configure session
        ArConfig *ar_config = nullptr;
        ArConfig_create(arSession, &ar_config);
        int32_t supported = 0;
        ArSession_isDepthModeSupported(arSession, AR_DEPTH_MODE_AUTOMATIC,
                                   &supported);
        if(supported){
            ArConfig_setDepthMode(arSession, ar_config, AR_DEPTH_MODE_AUTOMATIC);
            __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                                "ArSession Depth Mode activated");
        }else {
            ArConfig_setDepthMode(arSession, ar_config, AR_DEPTH_MODE_DISABLED);
            __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                                "ArSession Depth Mode disabled");
        }
        ArConfig_setInstantPlacementMode(arSession, ar_config,
        AR_INSTANT_PLACEMENT_MODE_LOCAL_Y_UP);
        if (ArSession_configure(arSession, ar_config) != AR_SUCCESS) {

            __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                                "ArSession couldn't be configured");
            return false;
        }
        ArConfig_destroy(ar_config);

        ArFrame_create(arSession, &arFrame);
        ArSession_setDisplayGeometry(arSession, displayRotation, width, height);
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "Created AR Session successfully");
        return true;
}

bool ArServer::isArCoreInstalled()
{
    bool userRequestedInstall = !installRequested;

    ArInstallStatus installStatus;
    // Ensure Google Play Services for AR and ARCore device profile data are
    // installed and up to date.
    ArStatus error = ArCoreApk_requestInstall(
            env, activity, userRequestedInstall, &installStatus);
    if (error != AR_SUCCESS) {
        // Inform user of error.
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "ArCore is not installed or up to date Error: %d", error);
        return false;
    }

    switch (installStatus) {
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
            installRequested = true;
            __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                                "Install ArCore");
            return false;
    }
    return true;
}

void ArServer::createAnchorAt(float x, float y) {


    if (ArSession_update(arSession, arFrame) == AR_SUCCESS) {
        ArHitResultList *hit_result_list = nullptr;
        ArHitResultList_create(arSession, &hit_result_list);
        ArFrame_hitTest(arSession, arFrame, x, y,
                                        hit_result_list);
        int32_t hit_result_list_size = 0;
        ArHitResultList_getSize(arSession, hit_result_list,
                                &hit_result_list_size);

        for (int i = 0; i< hit_result_list_size;i++) {
            ArHitResult *ar_hit = nullptr;
            ArHitResult_create(arSession, &ar_hit);
            ArHitResultList_getItem(arSession, hit_result_list, i, ar_hit);

            ArTrackable *ar_trackable = nullptr;
            ArHitResult_acquireTrackable(arSession, ar_hit, &ar_trackable);
            ArTrackableType ar_trackable_type = AR_TRACKABLE_NOT_VALID;
            ArTrackable_getType(arSession, ar_trackable, &ar_trackable_type);

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

            __android_log_print(ANDROID_LOG_VERBOSE, "Teampraktikum",
                                "Found AR_Trackable %s", type.c_str());

            if(ar_trackable_type==AR_TRACKABLE_POINT || ar_trackable_type==AR_TRACKABLE_INSTANT_PLACEMENT_POINT || ar_trackable_type == AR_TRACKABLE_PLANE){
                if (anchor != nullptr) {
                    ArAnchor_release(anchor);
                }
                ArHitResult_acquireNewAnchor(arSession, ar_hit, &anchor);
                __android_log_print(ANDROID_LOG_VERBOSE, "Teampraktikum",
                                    "Anchor %s created successfully", type.c_str());
            }
        }
        ArHitResultList_destroy(hit_result_list);


    }
}

ArAnchor * ArServer::getAnchor() {
    return anchor;
}


