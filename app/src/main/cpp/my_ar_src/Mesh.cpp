//
// Created by david on 17.04.21.
//

#include "Mesh.h"

Mesh::Mesh(std::string shaderName, std::string objectName , Scene* scene_) {
    this->scene = scene_;
    AAssetManager * assetMgr = scene->getAssetManager();
    myShader = new Shader(assetMgr);
    myShader->addShader("shader/"+shaderName+".f.glsl", GL_FRAGMENT_SHADER);
    myShader->addShader("shader/"+shaderName+".v.glsl", GL_VERTEX_SHADER);
    this->programID = myShader->linkShader();
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "Shaders are linked");

    std::vector<GLfloat> *  vertices = new std::vector<GLfloat>();
    std::vector<GLfloat> *  normals = new std::vector<GLfloat>();
    std::vector<GLfloat> *  uvs = new std::vector<GLfloat>();
    std::vector<GLushort> * indices = new std::vector<GLushort>();
    if (LoadObjFile("obj/"+objectName+".obj", assetMgr, vertices, normals, uvs, indices) == false) {

        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                            "cube.obj couldn't be loaded successfully");
        return;
    }
    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "cube.obj loaded successfully");

    glGenBuffers(3, vboID);
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices->size(), vertices->data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *) 0);


    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals->size(), normals->data(),
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * uvs->size(), uvs->data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, -1);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices->size(), indices->data(),
                 GL_STATIC_DRAW);

//    for (const auto &value: *indices) {
//        __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
//                            "Value : %f %f %f", vertices->at(value * 3 + 0),
//                            vertices->at(value * 3 + 1), vertices->at(value * 3 + 2));
//    }
//    glBindVertexArray(-1);

    indicesSize = indices->size();
//    glFlush();
//    delete vertices;
//    delete normals;
//    delete uvs;
//    delete indices;

    __android_log_print(ANDROID_LOG_VERBOSE, "TeamPraktikum",
                        "cube.obj uploaded to GL");
    mvpMatrixID = glGetUniformLocation(programID, "mvp");
    mvMatrixID = glGetUniformLocation(programID, "mv");
    viewMatrixID = glGetUniformLocation(programID, "view");
    baseColorID = glGetUniformLocation(programID, "baseColor");


}

Mesh::~Mesh() {

}

void Mesh::draw(glm::mat4 model,glm::vec4 color) {
    //__android_log_print(ANDROID_LOG_VERBOSE, "Teampraktikum","In draw in Mesh");
    glm::mat4 view = scene->getView();
    glm::mat4 projection = scene->getProjection();
    std::string viewString="";
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            viewString.append(std::to_string(model[j][i])+" ");
        }
        viewString.append("\n");
    }
    //__android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","ModelMatrix in Mesh:\n %s",viewString.c_str());
    glUseProgram(programID);
    updateVertexData();
    glm::mat4 mv = view * model;
    glm::mat4 mvp = projection*mv;
    glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(mvMatrixID, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(view));
    glUniform4fv(baseColorID,1,glm::value_ptr(color));
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_SHORT, (void *) 0);
}

void Mesh::updateVertexData() {
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, -1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}
