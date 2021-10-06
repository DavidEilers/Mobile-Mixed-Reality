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
    void update()override;

    void hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) override;

private:
    Node * fields[9];
    Mesh * crossMesh;
    Mesh * circleMesh;
    TTTSlave *slave;
    TTTMaster *master;
    TTTBoard *board;
    unsigned int playerType;
    bool isMaster;
};
#endif //TEAMPRAKTIKUM_TICTACTOESCENE_H
