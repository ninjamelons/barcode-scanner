cmake_minimum_required(VERSION 3.5)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

project(barcodescanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Preprocessing)
include_directories(SampleApps)
include_directories(Decoding)
include_directories(Tests)

add_subdirectory(Preprocessing)
add_subdirectory(SampleApps)
add_subdirectory(Decoding)
add_subdirectory(Tests)