//
// Created by david on 10/15/21.
//

#ifndef TEAMPRAKTIKUM_TEXTUREDQUAD_H
#define TEAMPRAKTIKUM_TEXTUREDQUAD_H
#include "ImageTexture.h"
#include "shader.h"

/**
 * @class TexturedQuad
 * A class for rendering a textured Quad
 */
class TexturedQuad {
public:

    /**
     * @brief The Constructor for Textured Quad
     * @param mgr AssetManager for reading files
     * @param imageName Name of the image so that assets/images/imageName represents the image to load
     * @param posLeftUpperX The x position in OpenGL screen-space of the upper left corner of the Quad
     * @param posLeftUpperY The y position in OpenGL screen-space of the upper left corner of the Quad
     * @param width The width in OpenGL screen-space
     * @param height The height in OpenGL screen-space
     */
    TexturedQuad(AAssetManager *mgr, std::string imageName, float posLeftUpperX, float posLeftUpperY, float width, float height);
    virtual ~TexturedQuad();

    /**
     * @brief Draw this screen-quad
     */
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
