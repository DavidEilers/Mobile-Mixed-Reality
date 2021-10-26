//
// Created by david on 17.04.21.
//

#ifndef TEAMPRAKTIKUM_SCENE_H
#define TEAMPRAKTIKUM_SCENE_H

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
#include "Mesh.h"
#include "Node.h"
#include "ext.hpp"


class Mesh;
class Node;

/**
 * @class Scene
 * A class for a scene-graph and rendering this scene-graph
 */
class Scene {
public:

    /**
     * @brief A Constructor for Scene
     * @param assetManager_  An AssetManager for reading Files
     */
    Scene(AAssetManager *assetManager_);
    virtual ~Scene();

    /**
     * @brief Set the model-matrix for the Scene
     * @param model_ A glm::mat4 model-matrix
     */
    void setModel(glm::mat4 model_);

    /**
     * @brief Set the view-matrix for the Scene
     * @param view_ A glm::mat4 view-matrix
     */
    void setView(glm::mat4 view_);

    /**
    * @brief Set the projection-matrix for the Scene
    * @param view_ A glm::mat4 projection-matrix
    */
    void setProjection(glm::mat4 projection_);

    /**
     * @brief Draws the Scene (rootNode)
     */
    virtual void draw();

    /**
     * @brief Get the view-matrix of the Scene
     * @return Returns glm::mat4 view-matrix
     */
    glm::mat4 getView();

    /**
     * @brief Get the projection-matrix of the Scene
     * @return Returns glm::mat4 projection-matrix
     */
    glm::mat4 getProjection();

    /**
    * @brief Get the assetManager of the Scene
    * @return Returns the assetManager
    */
    AAssetManager* getAssetManager();

    /**
     * @brief Expecting a hitTest function which handles hitTests
     * @param rayOrigin The Origin of the ray in world-space
     * @param rayDestination The direction of the ray in world-space
     */
    virtual void hitTest(glm::vec3 rayOrigin, glm::vec3 rayDestination)=0;

    /**
     * @brief Expecting a update function, which updates the Scene state
     */
    virtual void update()=0;


protected:
    Node* getHitTestNode(glm::vec3 rayOrigin, glm::vec3 rayDestination);
    AAssetManager *assetManager=nullptr;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    Node * rootNode= nullptr;

};


#endif //TEAMPRAKTIKUM_SCENE_H
