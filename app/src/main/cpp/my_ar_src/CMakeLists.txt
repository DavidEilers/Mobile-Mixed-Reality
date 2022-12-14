# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Import the ARCore library.
add_library(arcore SHARED IMPORTED)
set_target_properties(arcore PROPERTIES IMPORTED_LOCATION
        ${ARCORE_LIBPATH}/${ANDROID_ABI}/libarcore_sdk_c.so
        INTERFACE_INCLUDE_DIRECTORIES ${ARCORE_INCLUDE}
        )

# Import the glm header file from the NDK.
add_library( glm INTERFACE )
set_target_properties( glm PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${GLM_INCLUDE})


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        my-AR-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        #native-lib.cpp
        cameraBackground.cpp objLoader.cpp shader.cpp objRenderer.cpp arServer.cpp planeRenderer.cpp Mesh.cpp Node.cpp Scene.cpp BoundingBox.cpp TicTacToeScene.cpp FourInARowScene.cpp ImageTexture.cpp TexturedQuad.cpp MenueScene.cpp)

#add_library(cameraBackground SHARED cameraBackground.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

find_library( # Sets the name of the path variable.
        android-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        android )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
#find_library(networkLib network-lib)

target_include_directories (my-AR-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries( # Specifies the target library.
        my-AR-lib
        glm
        #cameraBackground
        arcore
        GLESv2
        network-lib
        #test-lib
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        ${android-lib})