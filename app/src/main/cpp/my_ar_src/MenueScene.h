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


class MenueScene: public Scene {
public:
    MenueScene(AAssetManager *assetManager);

    virtual ~MenueScene();

    virtual void hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) override;

    virtual void update() override;

    void draw() override;
    void setHaveWon(bool haveWon_);
    bool getShow();
    void setShow(bool show_);
    bool getPlayAgain();
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
