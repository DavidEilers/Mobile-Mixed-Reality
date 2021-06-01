//
// Created by david on 02.03.21.
//

#ifndef TEAMPRAKTIKUM_OBJLOADER_H
#define TEAMPRAKTIKUM_OBJLOADER_H
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>
#include <android/log.h>
#include <errno.h>
#include <jni.h>

#include <cstdint>
#include <cstdlib>
#include <map>
#include <vector>

#include "arcore_c_api.h"
//#include "glm.h"
#include <unistd.h>
#include <sstream>
#include <string>
#include <android/log.h>

//#include "jni_interface.h"

bool LoadTextFileFromAssetManager(const char* file_name,
                                  AAssetManager* asset_manager,
                                  std::string* out_file_text_string);

bool LoadObjFile(const std::string& file_name, AAssetManager* asset_manager,
                 std::vector<GLfloat>* out_vertices,
                 std::vector<GLfloat>* out_normals,
                 std::vector<GLfloat>* out_uv,
                 std::vector<GLushort>* out_indices);
#endif //TEAMPRAKTIKUM_OBJLOADER_H
