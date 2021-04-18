//
// Created by david on 17.04.21.
//

#include "Node.h"

Node::Node() {
    model = glm::mat4(1.0f);//identity matrix
    childs = new std::vector<Node*>();
}

void Node::setModel(glm::mat4 model_) {
    this->model = model_;
}

Node::~Node() {

}

void Node::addChild(Node* node) {
    childs->push_back(node);
}

void Node::setMesh(Mesh *mesh) {
    this->mesh = mesh;
}

void Node::draw(glm::mat4 parentTransform) {
    glm::mat4 newModel = parentTransform*model;
    if(mesh!= nullptr) {
        mesh->draw(newModel);
    }
    for(Node* e: *childs){
        e->draw(newModel);
    }
}
