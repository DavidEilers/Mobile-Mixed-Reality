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


class FourInARowScene:public Scene {
public:
    FourInARowScene(AAssetManager *assetManager);

    virtual ~FourInARowScene();

    void hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) override;

private:
    void update() override;
};


#endif //TEAMPRAKTIKUM_FOURINAROWSCENE_H
