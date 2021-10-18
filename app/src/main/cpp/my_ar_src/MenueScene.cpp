//
// Created by david on 10/15/21.
//

#include "MenueScene.h"


MenueScene::~MenueScene() {

}

void MenueScene::hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) {
    Node* result = this->getHitTestNode(rayOrigin, rayDestination);
//    if(result==exitHitBox){
//        __android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikumSceneMenue","Should Exit");
//        shouldExit=true;
//    }
    if(result==playAgainHitBox){
        show=false;
        playAgain=true;
    }
}

void MenueScene::update() {

}

glm::vec3* MenueScene::getHitBox(glm::vec2 posUpperLeft,float width,float height) {
    glm::vec2 ul = posUpperLeft;
    glm::vec2 ll = ul+glm::vec2(0,-height);
    glm::vec2 lr = ul +glm::vec2(width,-height);
    glm::vec2 ur = ul +glm::vec2(width,0);
    glm::vec3 *box = new glm::vec3[8]();
    box[0]=glm::vec3(ul.x,0.0,ul.y);
    box[1]=glm::vec3(ll.x,0.0,ll.y);
    box[2]=glm::vec3(lr.x,0.0,lr.y);
    box[3]=glm::vec3(ur.x,0.0,ur.y);
    box[4]=glm::vec3(0,0.06,0)+box[0];
    box[5]=glm::vec3(0,0.06,0)+box[1];
    box[6]=glm::vec3(0,0.06,0)+box[2];
    box[7]=glm::vec3(0,0.06,0)+box[3];
    return box;


}


MenueScene::MenueScene(AAssetManager *assetManager) : Scene(assetManager) {
    this->haveWon=false;
    this->playAgain=false;
    this->show=false;
    winQuad = new TexturedQuad(assetManager,"you_won.tga",-0.7f,0.5f,1.4f,0.28f);
    lostQuad = new TexturedQuad(assetManager,"you_lost.tga",-0.7f,0.5f,1.4f,0.28f);
    playAgainQuad = new TexturedQuad(assetManager,"play_again.tga",-0.7f,0.1f,1.4f,0.28f);
    playAgainHitBox = new Node(this);
    glm::vec3 * playAgainHitboxData = getHitBox(glm::vec2(-0.7f,0.1f),1.4f,0.28f);
    playAgainHitBox->setBoundingBox(playAgainHitboxData);
    this->rootNode->addChild(playAgainHitBox);
    //exitQuad = new TexturedQuad(assetManager,"exit.tga",-0.7f,-0.3f,1.4f,0.28f);
    //glm::vec3 * exitHitboxData = getHitBox(glm::vec2(-0.7f,-0.3f),1.4f,0.28f);
    //exitHitBox = new Node(this);
    //exitHitBox->setBoundingBox(exitHitboxData);
    //this->rootNode->addChild(exitHitBox);
    this->shouldExit=false;
}

void MenueScene::setHaveWon(bool haveWon_){
    this->haveWon = haveWon_;
}

void MenueScene::draw() {
    //__android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikumMenuScene","DRAWS MENUE SCENE");
    if(show) {
        Scene::draw();
        //winQuad->draw();
        if (haveWon) {
            winQuad->draw();
        } else {
            lostQuad->draw();
        }
        playAgainQuad->draw();
        //exitQuad->draw();
    }

}

bool MenueScene::getShow() {
    return show;
}

void MenueScene::setShow(bool show_) {
    this->show = show_;
}

bool MenueScene::getPlayAgain() {
    return playAgain;
}

bool MenueScene::getShouldExit() {
    return shouldExit;
}
