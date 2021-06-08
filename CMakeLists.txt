cmake_minimum_required(VERSION 3.5)

project(bscanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add OpenCV to project libraries
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Include header directories
include_directories(Preprocessing)
include_directories(Decoding)
include_directories(SampleApps)
include_directories(Testing)
include_directories(${PROJECT_SOURCE_DIR}) # Include .h header/type files in root

# Add subdirectory CMakeLists to make
add_subdirectory(Preprocessing)
add_subdirectory(Decoding)
add_subdirectory(SampleApps)
add_subdirectory(Testing)


add_library (Scanner scanner.cpp)
target_link_libraries( Scanner )
target_include_directories (Scanner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${OpenCV_LIBS})