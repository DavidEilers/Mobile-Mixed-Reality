//
// Created by david on 17.04.21.
//

#include "Node.h"

Node::Node(Scene * scene_) {
    model = glm::mat4(1.0f);//identity matrix
    childs = new std::vector<Node*>();
    this->scene = scene_;
    this->color = glm::vec4(1.0f,0.0f,0.0f,1.0f);
    prepareBoundingBoxShader();//TODO Delete this Bounding Box Shader
}

void Node::setModel(glm::mat4 model_) {
    this->model = model_;
}

Node::~Node() {

}

size_t Node::getChildCount() {
    return childs->size();
}

void Node::prepareBoundingBoxShader() {
    AAssetManager * assetMgr = scene->getAssetManager();
    boundingBoxShader = new Shader(assetMgr);
    boundingBoxShader->addShader("shader/simple.f.glsl", GL_FRAGMENT_SHADER);
    boundingBoxShader->addShader("shader/simple.v.glsl", GL_VERTEX_SHADER);
    this->programID = boundingBoxShader->linkShader();
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "Shaders are linked");

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3*8, nullptr,GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);


    mvpID = glGetUniformLocation(programID, "mvp");

}

void Node::addChild(Node* node) {
    childs->push_back(node);
}

void Node::setMesh(Mesh *mesh) {
    this->mesh = mesh;
}

void Node::renderBoundingBox(glm::mat4 model) {
    GLfloat *data = new GLfloat[8*3]();
    for(int i =0;i<8;i++){
        for(int j = 0; j<3; j++){
            data[i*3+j]=box[i][j];
        }
    }
    glUseProgram(programID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, (void *) 0);
    glBufferSubData(GL_ARRAY_BUFFER,0,8*3*sizeof(GLfloat),data);
    glm::mat4 modelview = scene->getProjection()*scene->getView()*model;
    glUniformMatrix4fv(mvpID,1, GL_FALSE, glm::value_ptr(modelview));
    glLineWidth(3.0f);
    glDrawArrays(GL_POINTS,0,8);
    glDrawArrays(GL_LINE_LOOP,0,4);
    glDrawArrays(GL_LINE_LOOP,4,4);

    //glDrawElements(GL_LINES,12,GL_UNSIGNED_SHORT, (void*)0);
    //glDrawElements(GL_POINTS,4,GL_UNSIGNED_SHORT, (void*)0);
    //__android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum", "BoundingBox Rendering");

}

void Node::draw(glm::mat4 parentTransform) {
    glm::mat4 newModel = parentTransform*model;
    if(mesh!= nullptr) {
        if(wasHit==false){
            mesh->draw(newModel,color);
        }else{
            mesh->draw(newModel,color);
        }

    }else if(box!= nullptr){
        renderBoundingBox(newModel);
    }
    for(Node* e: *childs){
        e->draw(newModel);
    }
}


void Node::setBoundingBox(glm::vec3 *box) {
    boundingBox = new BoundingBox();
    this->box = box;
}

Node* Node::hitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection,glm::mat4 parentTransform) {
    glm::mat4 newModel = parentTransform*model;

    __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","BoundingBox Test");
    if(box!= nullptr){
        glm::vec3 * correctBox = new glm::vec3[8]();
        for(int i=0; i<8;i++){
            correctBox[i]= glm::vec3(newModel*glm::vec4(box[i],1.0f));
        }
        boundingBox->setBoundingBox(correctBox);
        if(boundingBox->hitTest(rayOrigin,rayDirection)==true){
            delete [] correctBox;
            wasHit=true;
            __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","*****BoundingBox was hit******");
            return this;
        }
        delete [] correctBox;
        __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","BoundingBox wasn't hit");
    }

    for(Node* e: *childs){
        Node* result = e->hitTest(rayOrigin,rayDirection,newModel);
        if(result!=nullptr){return result;}
    }
    return nullptr;

}

void Node::clearChilds() {
    childs->clear();
}

void Node::setColor(glm::vec4 color_) {
    this->color = color_;
}
