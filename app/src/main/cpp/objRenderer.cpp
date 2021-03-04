//
// Created by david on 02.03.21.
//
#include "objRenderer.h"

ObjRenderer::ObjRenderer(std::string filename, AAssetManager *assetMgr) {
    myShader= new Shader(assetMgr);
    myShader->addShader("shader/objects.f.glsl",GL_FRAGMENT_SHADER);
    myShader->addShader("shader/objects.v.glsl",GL_VERTEX_SHADER);
    this->programID = myShader->compileShader();
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "Shaders are linked");


    vertices=new std::vector<GLfloat>();
    normals=new std::vector<GLfloat>();
    uvs = new std::vector<GLfloat>();
    indices = new std::vector<GLushort>();
    if(LoadObjFile("obj/cube.obj",assetMgr,vertices,normals,uvs,indices)==false){

        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "cube.obj couldn't be loaded successfully");
        return;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                         "cube.obj loaded successfully");
//    for(const auto& value: *vertices){
//        if(abs(value)>1.0f){
//            __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
//                                "%f bigger than 1.0f",value);
//        }
//    }
    glGenVertexArrays(1,&vao);
    glGenBuffers(3,vbo);
    glGenBuffers(1,&ibo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vertices->size(),vertices->data(),GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*3,(void*)0);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "VAO: %i Vertices.size: %i",vao,vertices->size());

    glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*normals->size(),normals->data(),GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    glBindBuffer(GL_ARRAY_BUFFER,vbo[2]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*uvs->size(),uvs->data(),GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,(void*)0);
    glBindBuffer(GL_ARRAY_BUFFER,-1);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*indices->size(),indices->data(),GL_STATIC_DRAW);
    //glDisableVertexAttribArray(vao);

    for(const auto& value: *indices){
        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                                "Value : %f %f %f",vertices->at(value*3+0),vertices->at(value*3+1),vertices->at(value*3+2));
    }
    glBindVertexArray(-1);


    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "cube.obj uploaded to GL");

}

void ObjRenderer::draw() {
    glBindVertexArray(vao);
    glUseProgram(programID);
    glDrawElements(GL_TRIANGLES,indices->size(),GL_UNSIGNED_SHORT,(void*)0);
    //glDrawArrays(GL_TRIANGLES,0,3);
    glBindVertexArray(-1);
}