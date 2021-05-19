cmake_minimum_required(VERSION 3.5)

project(barcodescanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Preprocessing)
include_directories(SampleApps)
include_directories(Decoding)
include_directories(Testing)

add_subdirectory(Preprocessing)
add_subdirectory(SampleApps)
add_subdirectory(Decoding)
add_subdirectory(Testing)