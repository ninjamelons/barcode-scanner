add_executable(sobel sobel.cpp)
add_executable(hough hough.cpp)
add_executable(edge edge.cpp)
add_executable(video video.cpp)

# Link OpenCV to executables
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( sobel ${OpenCV_LIBS} )
target_link_libraries( hough ${OpenCV_LIBS} )
target_link_libraries( edge ${OpenCV_LIBS} )
target_link_libraries( video ${OpenCV_LIBS} )

# Link preprocessing library
target_link_libraries (sobel LINK_PUBLIC Preprocessing)
target_link_libraries (hough LINK_PUBLIC Preprocessing)
target_link_libraries (edge LINK_PUBLIC Preprocessing)
target_link_libraries (video LINK_PUBLIC Preprocessing)

# Link decoding library
target_link_libraries (sobel LINK_PUBLIC Decoding)
target_link_libraries (hough LINK_PUBLIC Decoding)
target_link_libraries (edge LINK_PUBLIC Decoding)
target_link_libraries (video LINK_PUBLIC Decoding)