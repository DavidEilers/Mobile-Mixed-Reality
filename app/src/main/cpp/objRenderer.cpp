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
    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);
    glGenBuffers(1,&ibo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vertices->size(),vertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,sizeof(GLfloat)*3,GL_FLOAT,GL_FALSE,0,(void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort)*indices->size(),indices,GL_STATIC_DRAW);
    glDisableVertexAttribArray(vao);

}

void ObjRenderer::draw() {
    glUseProgram(programID);
    glEnableVertexAttribArray(vao);
    glDrawArrays(GL_TRIANGLES,0,indices->size());
    glDisableVertexAttribArray(vao);
}