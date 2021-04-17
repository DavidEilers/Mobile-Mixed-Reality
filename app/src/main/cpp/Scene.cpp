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
    std::string shaderName = "objects";
    std::string objectName = "cube";
    Mesh * mesh = new Mesh(shaderName,objectName,this);
    rootNode->setMesh(mesh);
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
