//
// Created by david on 10/15/21.
//

#ifndef TEAMPRAKTIKUM_TEXTUREDQUAD_H
#define TEAMPRAKTIKUM_TEXTUREDQUAD_H
#include "ImageTexture.h"
#include "shader.h"

class TexturedQuad {
public:
    TexturedQuad(AAssetManager *mgr, std::string imageName, float posLeftUpperX, float posLeftUpperY, float width, float heigth);
    virtual ~TexturedQuad();
    void draw();

private:
    void updateVertexData();
    struct __attribute__((packed)) Vertex{
        GLfloat x;
        GLfloat y;
        GLfloat u;
        GLfloat v;
    };
    Vertex quad[4];
    GLuint vboID;
    GLuint imageID;
    GLuint samplerLoc;
    ImageTexture* imageTexture;
    Shader* quadShader;
    GLuint programID;
};


#endif //TEAMPRAKTIKUM_TEXTUREDQUAD_H
