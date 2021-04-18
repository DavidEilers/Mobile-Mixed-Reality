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

Scene::Scene(AAssetManager *assetManager_) {
    this->assetManager = assetManager_;
    this->rootNode = new Node();
    Mesh * fieldMesh = new Mesh("objects","field",this);
    Node* fieldNode = new Node();
    rootNode->addChild(fieldNode);
    fieldNode->setMesh(fieldMesh);

    Mesh * crossMesh = new Mesh("objects","cross",this);
    Mesh * circleMesh = new Mesh("objects","circle",this);

    Node * fields[9];
    for(int i=0; i<9;i++){
        fields[i] = new Node();
        fieldNode->addChild(fields[i]);
        if(i%2==0){
            fields[i]->setMesh(circleMesh);
        }else{
            fields[i]->setMesh(crossMesh);
        }

        int row = (i/3);
        int column = i%3;
        double x = row*0.3-0.3;
        double z = column*0.3-0.3;
        glm::mat4 trans = glm::translate(glm::mat4(1.0f),glm::vec3(x,0.0f,z));
        fields[i]->setModel(trans);
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
    rootNode->setModel(model);
    glm::mat4 identityMatrix(1.0f);
    rootNode->draw(identityMatrix);
}
