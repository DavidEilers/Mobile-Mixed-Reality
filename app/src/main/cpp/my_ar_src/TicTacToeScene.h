//
// Created by david on 27.08.21.
//

#ifndef TEAMPRAKTIKUM_TICTACTOESCENE_H
#define TEAMPRAKTIKUM_TICTACTOESCENE_H
#import "Scene.h"
#include "../../../../../../../../../../programs/AndroidSDK/ndk/21.1.6352462/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/android/asset_manager.h"
#include "TicTacToe.hpp"
#include "MenueScene.h"

/**
 * @class TicTacToeScene
 * Implements the graphic presentation of "TicTacToe" and user-input
 */
class TicTacToeScene:public Scene {
public:

    MenueScene * menueScene;

    /**
     * @brief Constructor for TicTacToeScene
     * @param assetManager AssetManager for reading files
     * @param gamePointer Expecting a TicTacToeGame pointer for managing game-state
     */
    TicTacToeScene(AAssetManager *assetManager, jlong gamePointer);

    virtual ~TicTacToeScene();

    /**
     * @brief Updates the fields to represent the actual game-state
     */
    void update()override;

    /**
     * @brief Checks if a field was hit and sets X or O
     * @param rayOrigin The Origin of the ray in world-space
     * @param rayDestination The direction of the ray in world-space
     */
    void hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) override;

    /**
     * @brief Draws TicTacToe field
     */
    void draw() override;


private:
    void resetGame();
    Node * fields[9];
    Mesh * crossMesh;
    Mesh * circleMesh;
    TTTGame* gamePointer;
    TTTBoard *board;
    unsigned int playerType;
};
#endif //TEAMPRAKTIKUM_TICTACTOESCENE_H
