//
// Created by david on 17.05.21.
//

#include "BoundingBox.h"



BoundingBox::~BoundingBox() {

}

BoundingBox::BoundingBox(glm::vec3 boundingBoxMin, glm::vec3 boundingBoxMax)
        : boundingBoxMin(boundingBoxMin), boundingBoxMax(boundingBoxMax) {}

BoundingBox::BoundingBox(glm::vec3* boundingBox) {
    setBoundingBox(boundingBox);
}


void BoundingBox::setBoundingBox(glm::vec3 *box) {
    boundingBoxMin=box[0];
    boundingBoxMax=box[0];
    this->box = box;
    //find minimum and maximum for every vertice of boundingBox
    for(int i = 1; i<8; i++){

        //check x y z
        for(int j=0; j<3;j++){
            //check for minimum
            if(box[i][j]<boundingBoxMin[j]){
                boundingBoxMin[j]=box[i][j];
            }
            //check for maximum
            if(box[i][j]>boundingBoxMax[j]){
                boundingBoxMax[j]=box[i][j];
            }
        }
    }
}

//
//    _____ 2.
//   /___ /|
//   |   | |
// 1.|___|/
// 1. BoundingBoxMin
// 2. BoundingBoxMax

bool BoundingBox::hitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection) {

    //__android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","BoxMin: %f %f %f\nBoxMax: %f %f %f",boundingBoxMin.x,boundingBoxMin.y,boundingBoxMin.z,boundingBoxMax.x, boundingBoxMax.y, boundingBoxMax.z);
    // rayOrigin + factor* rayDirection = vec3(min.x,y,z)
    float factor = (boundingBoxMin.x-rayOrigin.x)/rayDirection.x;
    if(inBound(rayOrigin+factor*rayDirection)==true){
        return true;
    }
    // rayOrigin + factor* rayDirection = vec3(min.x,y,z)
    factor = (boundingBoxMax.x-rayOrigin.x)/rayDirection.x;
    if(inBound(rayOrigin+factor*rayDirection)==true){
        return true;
    }
    // rayOrigin + factor* rayDirection = vec3(x,min.y,z)
    factor = (boundingBoxMin.y-rayOrigin.y)/rayDirection.y;
    if(inBound(rayOrigin+factor*rayDirection)==true){
        return true;
    }
    // rayOrigin + factor* rayDirection = vec3(x,max.y,z)
    factor = (boundingBoxMax.y-rayOrigin.y)/rayDirection.y;
    if(inBound(rayOrigin+factor*rayDirection)==true){
        return true;
    }
    // rayOrigin + factor* rayDirection = vec3(x,y,min.z)
    factor = (boundingBoxMin.z-rayOrigin.z)/rayDirection.z;
    if(inBound(rayOrigin+factor*rayDirection)==true){
        return true;
    }
    // rayOrigin + factor* rayDirection = vec3(x,y,max.z)
    factor = (boundingBoxMax.z-rayOrigin.z)/rayDirection.z;
    if(inBound(rayOrigin+factor*rayDirection)==true){
        return true;
    }
    return false;
}

bool BoundingBox::inBound(glm::vec3 point) {
    float errorMargin = 0.0001;
    if(boundingBoxMin.x-errorMargin <point.x && point.x-errorMargin < boundingBoxMax.x
    && boundingBoxMin.y-errorMargin < point.y && point.y-errorMargin < boundingBoxMax.y
    && boundingBoxMin.z-errorMargin < point.z && point.z-errorMargin < boundingBoxMax.z
    ){
        return true;
    }else{
        return false;
    }
}
