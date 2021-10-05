//
// Created by david on 27.08.21.
//

#include "TicTacToeScene.h"
//#include "../../../../../../../../../../programs/AndroidSDK/ndk/21.1.6352462/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/jni.h"

TicTacToeScene::TicTacToeScene(AAssetManager *assetManager, const jlong &serverPointer,
                               const jboolean &isMaster) : Scene(assetManager, serverPointer,
                                                                 isMaster) {
}

TicTacToeScene::~TicTacToeScene() {

}
