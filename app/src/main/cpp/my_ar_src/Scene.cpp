//
// Created by david on 17.04.21.
//

#include "Scene.h"

void Scene::setModel( glm::mat4 model_) {
    this->model = model_;
}

void Scene::setView(glm::mat4 view_) {
    this->view = view_;
}

void Scene::setProjection(glm::mat4 projection_) {
    this->projection = projection_;
}

Scene::Scene(AAssetManager *assetManager_,std::string ip) {
    this->assetManager = assetManager_;
    this->rootNode = new Node(this);
    Mesh * fieldMesh = new Mesh("objects","field",this);
    Node* fieldNode = new Node(this);
    rootNode->addChild(fieldNode);
    fieldNode->setMesh(fieldMesh);

    crossMesh = new Mesh("objects","cross",this);
    circleMesh = new Mesh("objects","circle",this);
    slave.connect_to_master(ip,7080);
    slave.tick();
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

        if(slave.board.board[row][column]==1){
            fields[i]->setMesh(circleMesh);
        }else if(slave.board.board[row][column]==2){
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

}

AAssetManager* Scene::getAssetManager(){
    return assetManager;
}

Scene::~Scene() {

}

glm::mat4 Scene::getView() {
    return view;
}

glm::mat4 Scene::getProjection(){
    return projection;
}

void Scene::draw() {
    slave.tick();
    for(int i=0;i<9;i++){
        int row = (i/3);
        int column = i%3;
//        if(row==1&&column==1){
//            fields[i]->setMesh(circleMesh);
//        }
        switch(slave.board.board[row][column]){
            case 0:
                fields[i]->setMesh(nullptr);
                break;
            case 1:
                fields[i]->setMesh(crossMesh);
                break;
            case 2:
                fields[i]->setMesh(circleMesh);
                break;
            default:
                fields[i]->setMesh(nullptr);
                break;
        }
    }
    rootNode->setModel(model);
    glm::mat4 identityMatrix(1.0f);
    rootNode->draw(identityMatrix);
}

void Scene::hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination) {
    glm::vec4 rayOriginVS = glm::inverse(projection)*glm::vec4(rayOrigin,1.0f);
    //rayOriginVS.x /= rayOriginVS.w;
    //rayOriginVS.y /= rayOriginVS.w;
    //rayOriginVS.z /= rayOriginVS.w;
    glm::vec4 rayDirectionVS = glm::inverse(projection)*glm::vec4(rayDestination, 1.0f);
    //rayDirectionVS.x /= rayDirectionVS.w;
    //rayDirectionVS.y /= rayDirectionVS.w;
    //rayDirectionVS.z /= rayDirectionVS.w;
    rayDirectionVS.z = -1.0f;
    rayDirectionVS.w=0.0f;
    glm::vec3 rayOriginWS = glm::vec3(glm::inverse(view)*glm::vec4(0,0,0,1.0f));
    glm::vec3 rayDirectionWS =glm::normalize(glm::vec3(glm::inverse(view)*rayDirectionVS)); //glm::vec3(glm::inverse(view)*rayDirectionVS)-rayOriginWS;


    rootNode->setModel(model);
    glm::mat4 identityMatrix(1.0f);
    Node* result = rootNode->hitTest(rayOriginWS,rayDirectionWS,identityMatrix,view);
    int row;int column;
    for(int i=0; i<9; i++){
        if(fields[i]==result){
            row = (i/3);
            column = i%3;
            break;
        }
        if(i==8){
            return;//The result is no field
        }
    }
    slave.makeMove(row,column);

}
