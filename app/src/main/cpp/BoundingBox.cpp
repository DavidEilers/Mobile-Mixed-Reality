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
BoundingBox::BoundingBox() {
}


void BoundingBox::setBoundingBox(glm::vec3 *box) {
    boundingBoxMin=box[0];
    boundingBoxMax=box[0];
    this->box = box;
    for(int i=0;i<8;i++) {
        __android_log_print(ANDROID_LOG_VERBOSE, "Teampraktikum", "X:%.2f Y:%.2f Z:%.2f",box[i].x,box[i].y,box[i].z);
    }
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


    __android_log_print(ANDROID_LOG_VERBOSE, "Teampraktikum", "MINIMUM X:%.2f Y:%.2f Z:%.2f",boundingBoxMin[0],boundingBoxMin[1],boundingBoxMin[2]);
    __android_log_print(ANDROID_LOG_VERBOSE, "Teampraktikum", "MAXIMUM X:%.2f Y:%.2f Z:%.2f",boundingBoxMax[0],boundingBoxMax[1],boundingBoxMax[2]);
}

//
//    _____ 2.
//   /___ /|
//   |   | |
// 1.|___|/
// 1. BoundingBoxMin
// 2. BoundingBoxMax
//Implemented like suggested by https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

bool BoundingBox::hitTest(glm::vec3 rayOrigin, glm::vec3 rayDirection) {

    //__android_log_print(ANDROID_LOG_VERBOSE,"Teampraktikum","BoxMin: %f %f %f\nBoxMax: %f %f %f",boundingBoxMin.x,boundingBoxMin.y,boundingBoxMin.z,boundingBoxMax.x, boundingBoxMax.y, boundingBoxMax.z);

    //X Min Max

    // rayOrigin + factor* rayDirection = vec3(min.x,y,z)
    float factorXMin = (boundingBoxMin.x - rayOrigin.x) / rayDirection.x;
    // rayOrigin + factor* rayDirection = vec3(min.x,y,z)
    float factorXMax = (boundingBoxMax.x - rayOrigin.x) / rayDirection.x;

    if(factorXMin > factorXMax){
        std::swap(factorXMin, factorXMax);
    }

    //Y Min Max

    // rayOrigin + factor* rayDirection = vec3(x,min.y,z)
    float factorYMin = (boundingBoxMin.y-rayOrigin.y)/rayDirection.y;
    // rayOrigin + factor* rayDirection = vec3(x,max.y,z)
    float factorYMax = (boundingBoxMax.y-rayOrigin.y)/rayDirection.y;

    if(factorYMin> factorYMax){
        std::swap(factorYMin,factorYMax);
    }

    if((factorXMin>factorYMax)||(factorYMin>factorXMax)){
       return false;
    }

    if(factorYMin > factorXMin){
        factorXMin = factorYMin;
    }

    if(factorYMax < factorXMax){
        factorXMax = factorYMax;
    }
    // rayOrigin + factor* rayDirection = vec3(x,y,min.z)
    float factorZMin = (boundingBoxMin.z-rayOrigin.z)/rayDirection.z;
    // rayOrigin + factor* rayDirection = vec3(x,y,max.z)
    float factorZMax = (boundingBoxMax.z-rayOrigin.z)/rayDirection.z;

    if(factorZMin>factorZMax){
        std::swap(factorZMin,factorZMax);
    }

    if((factorXMin>factorZMax)||(factorZMin > factorXMax)){
        return false;
    }
    return true;
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
