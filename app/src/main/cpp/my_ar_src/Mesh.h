//
// Created by david on 17.04.21.
//

#ifndef TEAMPRAKTIKUM_MESH_H
#define TEAMPRAKTIKUM_MESH_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h> //NEED GLES2Extensions for GLES3 KNOWN WORKAROUND
#include <jni.h>
#include <string>
#include <vector>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "glm.hpp"
#include "ext.hpp"
#include "shader.h"
#include "objLoader.h"
#include "Scene.h"
#include "ext.hpp"

class Scene;

/**
 * @class Mesh
 * A class which represents a Mesh, which is uploaded to OpenGL and drawable
 */
class Mesh {
public:

    /**
     * @brief Constructor which Creates Mesh
     * @param shaderName The name of the Shader without file-extension which will read assets/shader/shaderName.f.glsl and assets/shader/shaderName.v.glsl
     * @param objectName The name of the obj file without file-extension which will load assets/obj/objectName.obj
     * @param scene_ The scene
     */
    Mesh(std::string shaderName, std::string objectName, Scene *scene_);
    virtual ~Mesh();

    /**
     * @brief Draws the Mesh with the provided shader
     * @param model modelViewProjection-matrix for the specified mesh
     * @param baseColor Color the mesh should have as RGBA
     */
    void draw(glm::mat4 model,glm::vec4 baseColor);

private:
    void updateVertexData();
    GLuint baseColorID;
    GLuint vboID[3];
    Shader *myShader;
    GLuint programID;
    GLuint ibo;
    GLuint mvpMatrixID;
    GLuint mvMatrixID;
    GLuint viewMatrixID;
    Scene* scene;
    size_t indicesSize;
};


#endif //TEAMPRAKTIKUM_MESH_H
