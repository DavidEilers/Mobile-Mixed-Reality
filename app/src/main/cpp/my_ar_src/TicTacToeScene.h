//
// Created by david on 27.08.21.
//

#ifndef TEAMPRAKTIKUM_TICTACTOESCENE_H
#define TEAMPRAKTIKUM_TICTACTOESCENE_H
#import "Scene.h"
#include "../../../../../../../../../../programs/AndroidSDK/ndk/21.1.6352462/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/android/asset_manager.h"

class TicTacToeScene:public Scene {
public:
    TicTacToeScene(AAssetManager *assetManager, const jlong &serverPointer,
                   const jboolean &isMaster);

    virtual ~TicTacToeScene();
};


#endif //TEAMPRAKTIKUM_TICTACTOESCENE_H
