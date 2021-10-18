//
// Created by david on 10/15/21.
//

#include "TexturedQuad.h"

TexturedQuad::TexturedQuad(AAssetManager *mgr, std::string imageName, float posLeftUpperX, float posLeftUpperY, float width, float height) {
    quad[0]=Vertex{posLeftUpperX,posLeftUpperY, 0.0f, 0.0f};
    quad[1]=Vertex{posLeftUpperX,posLeftUpperY-height,0.0f,1.0f};
    quad[2]=Vertex{posLeftUpperX+width,posLeftUpperY-height,1.0f,1.0f};
    quad[3]=Vertex{posLeftUpperX+width,posLeftUpperY,1.0f,0.0f};

    Vertex triangleData[6];
    triangleData[0]=quad[0];
    triangleData[1]=quad[1];
    triangleData[2]=quad[2];
    triangleData[3]=quad[0];
    triangleData[4]=quad[2];
    triangleData[5]=quad[3];

    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER,vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), &triangleData, GL_STATIC_DRAW);

    quadShader = new Shader(mgr);
    quadShader->addShader("shader/quadTexture.v.glsl",GL_VERTEX_SHADER);
    quadShader->addShader("shader/quadTexture.f.glsl",GL_FRAGMENT_SHADER);
    programID = quadShader->linkShader();
    glUseProgram(programID);
    samplerLoc = glGetUniformLocation(programID, "image");
    __android_log_print(ANDROID_LOG_VERBOSE,"TeamPraktikumTextureQuad","SamplerLoc: %d",samplerLoc );
    glUniform1i(samplerLoc, 0);

    imageTexture = new ImageTexture(mgr,imageName);
    glGenTextures(1,&imageID);
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D_ARRAY,imageID);
    glBindTexture(GL_TEXTURE_2D, imageID);
    imageTexture->uploadToGL();



}

void TexturedQuad::updateVertexData() {
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLfloat *) 0 + 2);
    glEnableVertexAttribArray(1);

}

void TexturedQuad::draw() {
    glUseProgram(programID);
    updateVertexData();
    //glEnable(GL_BLEND);
    //glBLend
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(samplerLoc, 0);
    glBindTexture(GL_TEXTURE_2D, imageID);
    //glBindTexture(GL_TEXTURE_EXTERNAL_OES, imageID);
    glDrawArrays(GL_TRIANGLES,0,6);
    //glDisable(GL_BLEND);

}

TexturedQuad::~TexturedQuad() {

}
