//
// Created by david on 06.10.21.
//

#include "FourInARowScene.h"
#include "../../../../../../../../../../programs/AndroidSDK/ndk/21.1.6352462/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/jni.h"

FourInARowScene::FourInARowScene(AAssetManager *assetManager) : Scene(assetManager) {
 Mesh* field = new Mesh("objects","fourInARowField",this);
 chip  = new  Mesh("objects","fourInARowChip",this);
 Node * fieldNode = new Node(this);
 rootNode->addChild(fieldNode);
 fieldNode->setMesh(field);
 fieldNode->setModel(
         glm::rotate(glm::mat4(1.0f),glm::radians(-90.0f),glm::vec3(1,0,0)));
 leftUpperMidOfBoundingBox = new Node(this);
 fieldNode->addChild(leftUpperMidOfBoundingBox);
 glm::vec3 leftUpperMidPos= glm::vec3(-0.1775,0,0.325);
 leftUpperMidOfBoundingBox->setModel(glm::translate(glm::mat4(1.0),leftUpperMidPos));
 //chipNode = new Node(this);
 //leftUpperMidOfBoundingBox->addChild(chipNode);
 //chipNode->setMesh(chip);

 glm::mat4 trans;

 glm::vec3 *box = new glm::vec3[8]();
 box[0] = glm::vec3(-0.025,-0.015,-0.025);
 box[1] = glm::vec3(0.025,-0.015,-0.025);
 box[2] = glm::vec3(0.025,-0.015,0.025);
 box[3] = glm::vec3(-0.025,-0.015,-0.025);
 box[4] = glm::vec3(-0.025,0.015,-0.025);
 box[5] = glm::vec3(0.025,0.015,-0.025);
 box[6] = glm::vec3(0.025,0.015,0.025);
 box[7] = glm::vec3(-0.025,0.015,-0.025);
 for(int i=0;i<8;i++){
  trans = glm::translate(glm::mat4(1.0f),glm::vec3(0.05*i,0,0));
  boundingBoxes[i] = new Node(this);
  boundingBoxes[i]->setModel(trans);
  boundingBoxes[i]->setBoundingBox(box);
  leftUpperMidOfBoundingBox->addChild(boundingBoxes[i]);
 }

 startPos = glm::vec3(0,0,-0.05);
 endPos = startPos+glm::vec3(0,0,-0.275);
 transformPos = startPos;

}

FourInARowScene::~FourInARowScene() {

}

void FourInARowScene::hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) {
  Node * hitBoxNode = getHitTestNode(rayOrigin,rayDestination);
  if(hitBoxNode->getChildCount()<=6&&animNode==nullptr) {
      animNode = new Node(this);
      animationAlpha = 0.0f;
      animNode->setMesh(chip);
      endPos = startPos+glm::vec3(0,0,-0.275+(0.05*hitBoxNode->getChildCount()));
      hitBoxNode->addChild(animNode);
  }

}

void FourInARowScene::update() {
 if(animNode!= nullptr) {
  animationAlpha += 0.1 * speed;
  speed *= 1.1;
  if (animationAlpha >= 1.0f) {
   speed=0.1f;
   animNode->setModel(glm::translate(glm::mat4(1.0f), endPos));
   animNode= nullptr;
   return;
  }

  transformPos = startPos * (1.0f - animationAlpha) + (endPos * animationAlpha);
  animNode->setModel(glm::translate(glm::mat4(1.0f), transformPos));
 }

}
