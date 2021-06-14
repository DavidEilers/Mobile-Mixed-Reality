# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

add_subdirectory("my_ar_src")
add_subdirectory("network_lib")


add_library(full-lib SHARED native-lib.cpp)


target_link_libraries(
        full-lib
        network-lib
        my-AR-lib
        ${log-lib}
        ${android-lib}
)