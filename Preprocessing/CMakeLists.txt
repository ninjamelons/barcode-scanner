# Add all libraries in preprocessing
add_library (Preprocessing process.cpp)

target_link_libraries( Preprocessing )

# Make sure the compiler can find include files for library
# when other libraries or executables link to library
target_include_directories (Preprocessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OpenCV_LIBS})