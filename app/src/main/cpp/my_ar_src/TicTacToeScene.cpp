//
// Created by david on 27.08.21.
//

#include "TicTacToeScene.h"
//#include "../../../../../../../../../../programs/AndroidSDK/ndk/21.1.6352462/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/jni.h"

TicTacToeScene::TicTacToeScene(AAssetManager *assetManager, jlong gamePointer_) : Scene(assetManager) {
    this->gamePointer = (TTTGame*)gamePointer_;
    playerType= PLAYER_X;

    Mesh * fieldMesh = new Mesh("objects","field",this);
    Node* fieldNode = new Node(this);
    rootNode->addChild(fieldNode);
    fieldNode->setMesh(fieldMesh);

    crossMesh = new Mesh("objects","cross",this);
    circleMesh = new Mesh("objects","circle",this);

    __android_log_print(ANDROID_LOG_VERBOSE,"TTTMaster","Before usage");
    board = gamePointer->board;
    __android_log_print(ANDROID_LOG_VERBOSE,"TTTMaster","After usage");
    //Node * fields[9];
    for(int i=0; i<9;i++){
        fields[i] = new Node(this);
        fieldNode->addChild(fields[i]);
//        if(i%2==0){
//            fields[i]->setMesh(circleMesh);
//        }else{
//            fields[i]->setMesh(crossMesh);
//        }

        int row = (i/3);
        int column = i%3;

        if(board->board[row][column]==1){
            fields[i]->setMesh(circleMesh);
        }else if(board->board[row][column]==2){
            fields[i]->setMesh(crossMesh);
        }
        double x = row*0.3-0.3;
        double z = column*0.3-0.3;
        glm::mat4 trans = glm::translate(glm::mat4(1.0f),glm::vec3(x,0.0f,z));
        fields[i]->setModel(trans);
        glm::vec3 *box = new glm::vec3[8]();
        box[0]=glm::vec3(-0.13,0.0,-0.13);
        box[1]=glm::vec3(0.13,0.0,-0.13);
        box[2]=glm::vec3(0.13,0.0,0.13);
        box[3]=glm::vec3(-0.13,0.0,0.13);
        box[4]=glm::vec3(0,0.06,0)+box[0];
        box[5]=glm::vec3(0,0.06,0)+box[1];
        box[6]=glm::vec3(0,0.06,0)+box[2];
        box[7]=glm::vec3(0,0.06,0)+box[3];
        fields[i]->setBoundingBox(box);
    }
    this->menueScene = new MenueScene(assetManager);

}

TicTacToeScene::~TicTacToeScene() {

}


void TicTacToeScene::update()  {
    //TTTBoard *board;
    //if(isMaster){board = &(master->board);}else{board = &(slave->board);}

    for(int i=0;i<9;i++){
        int row = (i/3);
        int column = i%3;
//        if(row==1&&column==1){
//            fields[i]->setMesh(circleMesh);
//        }
        switch(board->board[row][column]){
            case FIELD_EMPTY:
                fields[i]->setMesh(nullptr);
                break;
            case PLAYER_X:
                fields[i]->setMesh(crossMesh);
                break;
            case PLAYER_O:
                fields[i]->setMesh(circleMesh);
                break;
            default:
                fields[i]->setMesh(nullptr);
                break;
        }
    }
    if(board->check_win()==playerType){
        __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","You won!");
    }else if(board->check_win()!=0){
        __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","You loose!");
    }
    char tmp =gamePointer->checkWin();
    if(tmp!=0) {
        if (tmp == gamePointer->whoAmI()) {
            menueScene->setHaveWon(true);
            menueScene->setShow(true);
        } else {
            menueScene->setHaveWon(false);
            menueScene->setShow(true);
        }
    }

}

void TicTacToeScene::hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) {
    if(gamePointer->checkWin()==0) {
        Node *result = this->getHitTestNode(rayOrigin, rayDestination);
        int row=0;
        int column=0;
        for (int i = 0; i < 9; i++) {
            if (fields[i] == result) {
                row = (i / 3);
                column = i % 3;
                break;
            }
            if (i == 8) {
                return;//The result is no field
            }
        }
        __android_log_print(ANDROID_LOG_VERBOSE, "HitTest", "%d, %d", row, column);
        gamePointer->makeMove(row, column);
    }else {
        menueScene->hitTest(rayOrigin, rayDestination);
        if(menueScene->getPlayAgain()==true){
            gamePointer->restartGame();
            resetGame();
        }
    }

}

void TicTacToeScene::draw() {
    Scene::draw();
    menueScene->draw();
}

void TicTacToeScene::resetGame() {

}
