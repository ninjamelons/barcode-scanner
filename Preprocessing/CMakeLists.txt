list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Preprocessing/cmake")

# Add all libraries in preprocessing
add_library (Preprocessing process.cpp)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (Preprocessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OpenCV_LIBS})