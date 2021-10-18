#include <__bit_reference>
//
// Created by david on 10/15/21.
//

#ifndef TEAMPRAKTIKUM_IMAGETEXTURE_H
#define TEAMPRAKTIKUM_IMAGETEXTURE_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <exception>

struct __attribute__((packed)) TGAHeader {
    int8_t id_length;
    uint8_t color_map_type;
    int8_t image_type;
    int16_t color_map_start;
    int16_t color_map_length;
    uint8_t color_map_entry_size;
    int16_t x_coord_start;
    int16_t y_coord_start;
    int16_t image_width;
    int16_t image_height;
    uint8_t bits_per_pixel;
    uint8_t image_attribute_byte;
};

class ImageTexture {
public:
    ImageTexture(AAssetManager *mgr, std::string imageName);
    void uploadToGL();
private:

    TGAHeader* fileHeader;
    char * pixelData;
    size_t pixelDataLength;
    virtual ~ImageTexture();
};


#endif //TEAMPRAKTIKUM_IMAGETEXTURE_H
