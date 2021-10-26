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
#include "FourInARow.hpp"
#include "MenueScene.h"

/**
 * @class FourInARowScene
 * Implements the graphic presentation of "Four in a row" and user-input
 */
class FourInARowScene:public Scene {
public:
    MenueScene* menueScene;

    /**
     * @brief Constructor for FourInARowScene
     * @param assetManager For reading shader and images
     * @param gamePointer Expecting a FourGame pointer for managing game-state
     */
    FourInARowScene(AAssetManager *assetManager, jlong& gamePointer);

    virtual ~FourInARowScene();

    /**
     * @brief Does a hitTest against the scene
     * @param rayOrigin The Origin of the ray in world-space
     * @param rayDestination The direction of the ray in world-space
     */
    void hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) override;

    /**
     * @brief Draws the Scene
     */
    void draw() override;

private:
    void resetGame();
    void updateField();
    void update() override;
    float animationAlpha=0.0f;
    float speed=0.1f;
    glm::vec3 startPos;
    glm::vec3 endPos;
    glm::vec3 transformPos;
    Node * leftUpperMidOfBoundingBox;
    Node * lowerLeftField;
    Node * boundingBoxes[8];
    Node* fourInArowFields[48];
    Node * animNode;
    Mesh* chip;
    FourGame* gamePointer;

};


#endif //TEAMPRAKTIKUM_FOURINAROWSCENE_H
