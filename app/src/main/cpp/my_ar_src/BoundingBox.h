//
// Created by david on 17.05.21.
//

#ifndef TEAMPRAKTIKUM_BOUNDINGBOX_H
#define TEAMPRAKTIKUM_BOUNDINGBOX_H

#include "glm.hpp"
#include "ext.hpp"
#include <android/log.h>
#include <algorithm>


/**
 * @class BoundingBox
 * Class for representing a Bounding Box and doing hitTests
 */
class BoundingBox {

public:
    /**
     * @brief Constructor of BoundingBox
     * @param min Minimum (x,y,z) position of the bounding-box-vertices
     * @param max Maximum (x,y,z) positions of the bounding-box-vertices
     */
    BoundingBox(glm::vec3 min,glm::vec3 max);

    /**
     * @brief Constructor of the bounding-box
     * @param boundingBox A glm::vec3[8] where every vec3 represents a vertex of the box
     */
    BoundingBox(glm::vec3[8] );

    /**
     * @brief Default constructor of the bounding-box, Box must be set manually
     */
    BoundingBox();

    /**
     * @brief Checks if a given Ray hits the hit-box
     * @param rayOrigin The Origin of the ray in world-space
     * @param rayDirection The direction of the ray in world-space
     * @return Returns true if ray hits bounding-box else false
     */
    bool hitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection);

    /**
     * @brief Checks if a point is inside the bounding box
     * @param point The point to check against in world-space
     * @return Returns true if point is inside bounding-box else false
     */
    bool inBound(glm::vec3 point);

    /**
     * @brief Sets the bounding-box corners
     * @param box A glm::vec3[8] where every vec3 represents a vertex of the box
     */
    void setBoundingBox(glm::vec3 box[8]);
    virtual ~BoundingBox();


private:
    glm::vec3 *box;
    glm::vec3 boundingBoxMin;
    glm::vec3 boundingBoxMax;
};


#endif //TEAMPRAKTIKUM_BOUNDINGBOX_H
