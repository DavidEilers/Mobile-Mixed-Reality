//
// Created by david on 14.01.21.
//

#include "planeRenderer.h"

PlaneRenderer::PlaneRenderer(AAssetManager *mgr) {
    this->assetManager = mgr;
    initGL();
}

PlaneRenderer::~PlaneRenderer() {

}

void PlaneRenderer::initGL() {
    glGenBuffers(1,&vboID);
    glGenBuffers(1,&iboID);
    glBindBuffer(GL_ARRAY_BUFFER,vboID);
    glBufferData(GL_ARRAY_BUFFER,VBO_SIZE*sizeof(GLfloat), nullptr,GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,VBO_SIZE*sizeof(GLushort),nullptr,GL_DYNAMIC_DRAW);
    initShader();
}


void PlaneRenderer::initShader() {

    this->myShader = new Shader(assetManager);
    myShader->addShader("shader/grid.f.glsl", GL_FRAGMENT_SHADER);
    myShader->addShader("shader/grid.v.glsl", GL_VERTEX_SHADER);
    programID = myShader->linkShader();
    glUseProgram(programID);
    //glUseProgram(-1);
    mvpMatrixID = glGetUniformLocation(programID, "mvp");

}

void PlaneRenderer::setVertexData(std::vector<GLfloat>* vertices_){
    if(vertices!= nullptr){
        delete vertices;
    }
    if(indices!= nullptr){
        delete indices;
    }
    indices = new std::vector<GLushort>();
    this->vertices = vertices_;
    if(vertices->size() > VBO_SIZE){
        __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","Too many vertices! VBO overflow!");
    }else{
        glBindBuffer(GL_ARRAY_BUFFER,vboID);
        glBufferSubData(GL_ARRAY_BUFFER,0,vertices->size()*sizeof(GLfloat),vertices->data());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iboID);
        for(int i=0;i<(vertices->size()-3)/3;i++){
            indices->push_back(i*3);
            indices->push_back(((i+1)*3)%(vertices->size()));
            indices->push_back(vertices->size()-3);
        }
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,sizeof(GLushort)*indices->size(),indices->data());

        __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","First three vertices: %2.2f %2.2f %2.2f",vertices->at(0),vertices->at(1),vertices->at(2));
        __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","First three vertices: %2.2f %2.2f %2.2f",vertices->at(3),vertices->at(4),vertices->at(5));
        __android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","First three vertices: %2.2f %2.2f %2.2f",vertices->at(6),vertices->at(7),vertices->at(8));
    }
}



void PlaneRenderer::updateVertexData() {
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iboID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void PlaneRenderer::draw() {

    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum", "Drawing Plane");
    glUseProgram(programID);
    glm::mat4 mvp = projection*view*model;
    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, glm::value_ptr(mvp));
    updateVertexData();
    //glPointSize(10);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES,indices->size(),GL_UNSIGNED_SHORT,(void*) 0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum", "Drawn Plane with %i vertices",vertices->size());

}

void PlaneRenderer::setModelMatrix(glm::mat4 model_mat) {
    this->model = model_mat;
}

void PlaneRenderer::setViewMatrix(glm::mat4 view_mat) {
    this->view = view_mat;
}

void PlaneRenderer::setProjectionMatrix(glm::mat4 projection_mat) {
    this->projection = projection_mat;
}
