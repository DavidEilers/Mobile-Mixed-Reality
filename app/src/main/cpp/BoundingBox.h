//
// Created by david on 17.05.21.
//

#ifndef TEAMPRAKTIKUM_BOUNDINGBOX_H
#define TEAMPRAKTIKUM_BOUNDINGBOX_H

#include "glm.hpp"
#include "ext.hpp"
#include <android/log.h>


class BoundingBox {

public:
    BoundingBox(glm::vec3 min,glm::vec3 max);
    BoundingBox(glm::vec3[8]);
    bool hitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection);
    bool inBound(glm::vec3 point);
    void setBoundingBox(glm::vec3 box[8]);
    virtual ~BoundingBox();


private:
    glm::vec3 *box;
    glm::vec3 boundingBoxMin;
    glm::vec3 boundingBoxMax;
};


#endif //TEAMPRAKTIKUM_BOUNDINGBOX_H
