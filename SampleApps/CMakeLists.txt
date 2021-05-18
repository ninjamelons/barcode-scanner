list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/SampleApps/cmake")

add_executable(sobel sobel.cpp)
add_executable(hough hough.cpp)
add_executable(edge edge.cpp)
add_executable(video video.cpp)

# Link OpenCV to executables
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( sobel ${OpenCV_LIBS} Preprocessing Decoding )
target_link_libraries( hough ${OpenCV_LIBS} Preprocessing Decoding )
target_link_libraries( edge ${OpenCV_LIBS} Preprocessing Decoding )
target_link_libraries( video ${OpenCV_LIBS} Preprocessing Decoding )