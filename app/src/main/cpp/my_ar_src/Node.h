//
// Created by david on 17.04.21.
//

#ifndef TEAMPRAKTIKUM_NODE_H
#define TEAMPRAKTIKUM_NODE_H

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
#include "Mesh.h"
#include "objLoader.h"
#include "BoundingBox.h"
#include "ext.hpp"

class Mesh;
class Scene;

/**
 * @class Node
 * A drawable Node in a tree-structure
 */
class Node {
public:

    /**
     * @brief Constructor for Node
     * @param scene_ The Scene
     */
    Node(Scene* scene_);
    virtual ~Node();

    /**
     * @brief Set the model-matrix relative to it's parent Node
     * @param model_ Relative model-matrix
     */
    void setModel(glm::mat4 model_);

    /**
     * @brief Adds a childNode
     * @param node The Node which should be added
     */
    void addChild(Node* node);

    /**
     * @brief Get the amount of childs of this Node(only including direct childs)
     * @return The amount of direct childs
     */
    size_t getChildCount();

    /**
     * @brief Draws the managed Mesh if exist and our child Nodes
     * @param parentTransform The transform-matrix(model-view-projection) of the parent Node or view-projection-matrix for rootNode
     */
    void draw(glm::mat4 parentTransform);

    /**
     * @brief Set the Mesh to be rendered
     * @param mesh Mesh which should be rendered
     */
    void setMesh(Mesh *mesh);

    /**
     * @brief Set a boundingBox for this Node
     * @param box A glm::vec3[8] where every vec3 represents a vertex of the box
     */
    void setBoundingBox(glm::vec3 *box);

    /**
     * @brief Prepares shader if bounding box should be drawn(debugging)
     */
    void prepareBoundingBoxShader();

    /**
     * @brief Renders the bounding Box
     * @param model model-view-projection Matrix of this Node
     */
    void renderBoundingBox(glm::mat4 model);

    /**
     * @brief Delete all child Nodes
     */
    void clearChilds();

    /**
     * @brief Does a hitTest against our bounding-box and child Bounding Boxes
     * @param rayOrigin The Origin of the ray in world-space
     * @param rayDirection The direction of the ray in world-space
     * @param parentTransform Model-transform of the parentNode
     * @return Returns our Node if our bounding-box was hit, returns childNode if childNode's bounding-box was hit, else returns nullptr
     */
    Node* hitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection,glm::mat4 parentTransform);

    /**
     * @brief Sets the Color of the Mesh
     * @param color_ Color in RGBA
     */
    void setColor(glm::vec4 color_);

private:
    glm::vec4 color;
    GLuint programID;
    GLuint mvpID;
    GLuint vboID;
    GLuint iboID;
    Scene *scene=nullptr;
    Shader* boundingBoxShader=nullptr;
    bool wasHit=false;
    glm::vec3 *box=nullptr;
    BoundingBox * boundingBox=nullptr;
    glm::mat4 model;
    std::vector<Node*>* childs;
    Mesh* mesh=nullptr;

};



#endif //TEAMPRAKTIKUM_NODE_H
