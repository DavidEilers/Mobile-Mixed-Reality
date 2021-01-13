#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <jni.h>
#include <string>
#include "arcore_c_api.h"
#include <android/log.h>


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
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "Created AR Session successfully");
    }

    // Normal onResume behavior
    const ArStatus status = ArSession_resume(ar_session_);
    if (status != AR_SUCCESS) {
        return;
    }
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
}

JNIEXPORT void JNICALL
Java_com_example_teampraktikum_MainActivity_onDrawFrame(
        JNIEnv *env,
        jobject activity) {
    //glClearColor(0.7f,0.0f,0.0f,1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
}
}




