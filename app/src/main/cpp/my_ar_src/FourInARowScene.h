//
// Created by david on 06.10.21.
//

#ifndef TEAMPRAKTIKUM_FOURINAROWSCENE_H
#define TEAMPRAKTIKUM_FOURINAROWSCENE_H
#pragma once
#include "Scene.h"
#include "../../../../../../../../../../programs/AndroidSDK/ndk/21.1.6352462/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/android/asset_manager.h"
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


class FourInARowScene:Scene {
public:
    FourInARowScene(AAssetManager *assetManager, const jlong &serverPointer,
                    const jboolean &isMaster);

    virtual ~FourInARowScene();
};


#endif //TEAMPRAKTIKUM_FOURINAROWSCENE_H
