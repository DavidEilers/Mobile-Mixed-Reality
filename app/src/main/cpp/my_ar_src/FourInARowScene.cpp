//
// Created by david on 06.10.21.
//

#include "FourInARowScene.h"

FourInARowScene::FourInARowScene(AAssetManager *assetManager, jlong& gamePointer_) : Scene(assetManager) {
 this->gamePointer= (FourGame *) gamePointer_;
 this->animNode= nullptr;
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

 lowerLeftField = new Node(this);
 glm::vec3 lowerLeftFieldPos = glm::vec3(-0.1775,0,0);
 fieldNode->addChild(lowerLeftField);
 lowerLeftField->setModel(glm::translate(glm::mat4(1.0),lowerLeftFieldPos));
 glm::vec3 upOneField = glm::vec3(0,0,0.0505);
 glm::vec3 rightOneField= glm::vec3(0.0505,0,0);

 //width of field
 for(int i=0;i<8;i++){
  //height of field
  for(int j=0;j<6;j++){
   Node * newNode = new Node(this);
   fourInArowFields[i*6+j]= newNode;
   glm::vec3 pos = upOneField*(j+0.0f)+rightOneField*(i+0.0f);
   newNode->setModel(glm::translate(glm::mat4(1.0),pos));
   //newNode->setMesh(chip);
   lowerLeftField->addChild(newNode);
  }
 }
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
  int number = -1;
  for(int i=0;i<8;i++){
   if(boundingBoxes[i]==hitBoxNode)number=i;
  }
  if(number==-1){
   __android_log_print(ANDROID_LOG_VERBOSE,"TeampraktikumHitTest","NOT IN BOUNDINGBOX ARRAY");
  }
  if(number!=-1) {

   __android_log_print(ANDROID_LOG_VERBOSE,"TeampraktikumHitTest","Clicked Field %d",number);
   int amountInRow = gamePointer->board->amountInRow(number);
   if(gamePointer->myTurn()) {
    char player = gamePointer->whoAmI();
    gamePointer->makeMove(number);
    if (amountInRow <= 6 && animNode == nullptr) {
     animNode = new Node(this);
     animationAlpha = 0.0f;
     animNode->setMesh(chip);
     endPos = startPos + glm::vec3(0, 0, -0.275 + (0.05 * amountInRow));
     hitBoxNode->addChild(animNode);
     if(player==PLAYER_1){
      animNode->setColor(glm::vec4(1.0f,0.0f,0.0f,1.0f));
     }else{
       animNode->setColor(glm::vec4(1.0f,1.0f,0.0f,1.0f));
     }
    }
   }else{
     //try turn for debugging to work
     gamePointer->makeMove(number);
   }
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
   for(int i=0;i<8; i++){
    boundingBoxes[i]->clearChilds();
   }
   return;
  }

  transformPos = startPos * (1.0f - animationAlpha) + (endPos * animationAlpha);
  animNode->setModel(glm::translate(glm::mat4(1.0f), transformPos));
 }else{
  updateField();
 }

}

void FourInARowScene::updateField() {
 std::string a;
 for(int i = 0; i<8;i++){
  for(int j = 0; j<6; j++){
    if(gamePointer->board->get(i,5-j)==PLAYER_1){
     a.append("X");
      fourInArowFields[i*6+j]->setMesh(chip);
     fourInArowFields[i*6+j]->setColor(glm::vec4(1.0f,0.0f,0.0f,1.0f));
    }
    else if(gamePointer->board->get(i,5-j)==PLAYER_2){
     a.append("O");
     fourInArowFields[i*6+j]->setMesh(chip);
     fourInArowFields[i*6+j]->setColor(glm::vec4(1.0f,1.0f,0.0f,1.0f));
    }
    else{
     a.append("_");
    }
  }
  a.append(";");
 }
 __android_log_print(ANDROID_LOG_VERBOSE,"TeampraktikumFieldUpdate","%s",a.c_str());
}
