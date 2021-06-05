# Add all libraries in decoding
add_library (Decoding multiDecoder.cpp decoder.cpp code39.cpp code128.cpp ean13.cpp ean8.cpp)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( Decoding ${OpenCV_LIBS} )

target_include_directories (Decoding PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})