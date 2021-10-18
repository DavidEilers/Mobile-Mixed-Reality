//
// Created by david on 10/15/21.
//

#include "ImageTexture.h"

#include <android/log.h>
#include <iostream>
#include <cctype>

ImageTexture::ImageTexture(AAssetManager *mgr, std::string imageName) {
    std::string path("images/"+imageName);
    AAsset *file = AAssetManager_open(mgr, path.c_str(), AASSET_MODE_BUFFER);
    size_t fileSize = AAsset_getLength(file);

    //Save data from tga file into byteArray
    unsigned char* tgaData= new unsigned char[fileSize];
    //AAsset_
    AAsset_read(file,tgaData,fileSize);
    AAsset_close(file);
    //Move Headerdata into TGAHeader struct

    fileHeader = new TGAHeader;
    fileHeader->id_length = (int8_t)*(&tgaData[offsetof(TGAHeader,id_length)]);
    fileHeader->color_map_type = (int8_t)*(&tgaData[offsetof(TGAHeader,color_map_type)]);
    fileHeader->image_type = (int8_t)*(&tgaData[offsetof(TGAHeader,image_type)]);
    fileHeader->color_map_start = (int8_t)*(&tgaData[offsetof(TGAHeader,color_map_start)]);
    fileHeader->color_map_length = (int8_t)*(&tgaData[offsetof(TGAHeader,color_map_length)]);
    fileHeader->color_map_entry_size = (int8_t)*(&tgaData[offsetof(TGAHeader,color_map_entry_size)]);
    fileHeader->x_coord_start = (int16_t)*(&tgaData[offsetof(TGAHeader,x_coord_start)]);
    fileHeader->y_coord_start = (int16_t)*(tgaData+offsetof(TGAHeader,y_coord_start));
    fileHeader->image_width = (int16_t)*((int16_t*)(tgaData+offsetof(TGAHeader,image_width)));
    fileHeader->image_height = (int16_t)*(tgaData+offsetof(TGAHeader,image_height));
    fileHeader->bits_per_pixel = (int8_t)*(&tgaData[offsetof(TGAHeader,bits_per_pixel)]);
    fileHeader->image_attribute_byte = (int8_t)*(&tgaData[offsetof(TGAHeader,image_attribute_byte)]);

    //std::memcpy((void*)fileHeader,(void*)tgaData,sizeof(fileHeader));
    //calculate the offset of the beginning of the pixelData
    size_t imageOffset = 0;
    imageOffset+=sizeof(TGAHeader);
    imageOffset+=fileHeader->id_length;
    if(fileHeader->color_map_type==1){
        imageOffset+=fileHeader->color_map_length;
    }
    pixelDataLength = (fileHeader->image_width)*(fileHeader->image_height)*(fileHeader->bits_per_pixel/8);
    pixelData = new char[pixelDataLength];

    __android_log_print(ANDROID_LOG_VERBOSE,"TeampraktikumImageTexture","Id length start = %p",offsetof(TGAHeader,id_length));
    __android_log_print(ANDROID_LOG_VERBOSE,"TeampraktikumImageTexture","color map start = %p",offsetof(TGAHeader,color_map_start));
    __android_log_print(ANDROID_LOG_VERBOSE,"TeampraktikumImageTexture","Y coord start = %p",offsetof(TGAHeader,y_coord_start));
    __android_log_print(ANDROID_LOG_VERBOSE,"TeampraktikumImageTexture","Image width = %p",offsetof(TGAHeader,image_width));
    __android_log_print(ANDROID_LOG_VERBOSE, "TeampraktikumImageTexture", "Header: id_length: %d color_map_length: %d imageType: %d",fileHeader->id_length,fileHeader->color_map_length,fileHeader->image_type);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeampraktikumImageTexture", "Header: color_map_start: %d colormap_entry_size: %d x_coord_start: %d y_coord_start: %d",fileHeader->color_map_start,fileHeader->color_map_entry_size,fileHeader->x_coord_start,fileHeader->y_coord_start);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeampraktikumImageTexture", "Header: id_length: %d color_map_length: %d imageType: %d",fileHeader->id_length,fileHeader->color_map_length,fileHeader->image_type);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeampraktikumImageTexture", "Image: width: %d height: %hd bits_per_pixel: %d",fileHeader->image_width,fileHeader->image_height,fileHeader->bits_per_pixel);
    __android_log_print(ANDROID_LOG_VERBOSE, "TeampraktikumImageTexture", "Image: ImageOffset: %zu pixelDataLength: %zu Filesize: %zu",imageOffset,pixelDataLength,fileSize);

    if(imageOffset+pixelDataLength>fileSize){
        throw std::runtime_error("Filesize smaller than expected image data");
    }
    std::memcpy(pixelData,&tgaData[imageOffset],pixelDataLength);
    delete []tgaData;
}

void ImageTexture::uploadToGL() {
    /*jbyte* byteArray = new jbyte[pixelDataLength];
    for(size_t i=0;i<pixelDataLength;i++){
        byteArray[i]=pixelData[i];
    }

    jbyteArray imageOut = jni_interface_env->NewByteArray(pixelDataLength);
    jni_interface_env->SetByteArrayRegion(imageOut,0,pixelDataLength,byteArray);

    jclass helpClass = static_cast<jclass>(jni_interface_env->NewGlobalRef(jni_interface_env->FindClass("com/example/teampraktikum/JniInterface")));
    jmethodID helpMethod = static_cast<jmethodID>(jni_interface_env->GetStaticMethodID(helpClass,"loadTexture", "(III[B)V"));*/

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fileHeader->image_width, fileHeader->image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

   //jni_interface_env->CallStaticVoidMethod(helpClass,helpMethod,(jint)GL_TEXTURE_2D,(jint)fileHeader->image_width,(jint)fileHeader->image_height,imageOut);
}

ImageTexture::~ImageTexture() {
    delete []fileHeader;
    delete []pixelData;
}
