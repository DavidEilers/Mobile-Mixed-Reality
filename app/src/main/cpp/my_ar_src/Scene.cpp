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
    this->rootNode = new Node(this);


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
    update();
    rootNode->setModel(model);
    glm::mat4 identityMatrix(1.0f);
    rootNode->draw(identityMatrix);
}


Node* Scene::getHitTestNode(glm::vec3 rayOrigin, glm::vec3 rayDestination) {
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
    return result;
}

