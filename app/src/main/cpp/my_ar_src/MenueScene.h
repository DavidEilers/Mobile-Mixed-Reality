//
// Created by david on 10/15/21.
//

#ifndef TEAMPRAKTIKUM_MENUESCENE_H
#define TEAMPRAKTIKUM_MENUESCENE_H
#include "Scene.h"
#include "arServer.h"
#include "BoundingBox.h"
#include "cameraBackground.h"
#include "TexturedQuad.h"
#include <android/native_activity.h>


/**
 * @class MenueScene
 * Implements a Menue which shows if you Lost/Won/Draw and a Button for restarting the game
 */
class MenueScene: public Scene {
public:

    /**
     * @brief Constructor for MenueScene
     * @param assetManager AssetManager for loading images for button and lost/won/draw
     */
    MenueScene(AAssetManager *assetManager);

    virtual ~MenueScene();

    /**
     * @brief Does a hitTest against the buttons with a ray
     * @param rayOrigin The Origin of the ray in world-space
     * @param rayDestination The direction of the ray in world-space
     */
    virtual void hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) override;

    /**
     * @brief Does nothing, but is expected by Scene
     */
    virtual void update() override;

    /**
     * @brief Draws menue
     */
    void draw() override;

    /**
     * @brief Set if someone won or lost the game
     * @param haveWon_ Expected to be true if one and false if lost
     */
    void setHaveWon(bool haveWon_);

    /**
     * @brief Returns if the menue is shown
     * @return Returns true if the menue is shown and false if not
     */
    bool getShow();

    /**
     * @brief Set if the menue should be shown
     * @param show_ Set True if Menue should be shown or false if not
     */
    void setShow(bool show_);

    /**
     * @brief Returns if the Player wants to play again
     * @return Returns true if the game should reset else false
     */
    bool getPlayAgain();

    /**
     * @brief Returns true if the Player wants to Exit the game
     * @return Returns true if the App should exit else false
     */
    bool getShouldExit();

private:
    glm::vec3* getHitBox(glm::vec2 posUpperLeft,float width,float height);
    TexturedQuad* winQuad;
    TexturedQuad* lostQuad;
    TexturedQuad* playAgainQuad;
    TexturedQuad* exitQuad;
    Node* playAgainHitBox;
    Node* exitHitBox;
    bool haveWon;
    bool playAgain;
    bool show;
    bool shouldExit;
};


#endif //TEAMPRAKTIKUM_MENUESCENE_H
