cmake_minimum_required(VERSION 3.5)

project(opencv LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sobel sobel.cpp)
add_executable(hough hough.cpp)
add_executable(edge edge.cpp)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( sobel ${OpenCV_LIBS} )
target_link_libraries( hough ${OpenCV_LIBS} )
target_link_libraries( edge ${OpenCV_LIBS} )