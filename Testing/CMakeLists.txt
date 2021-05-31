set (WILDCARD_SOURCE *.test.cpp)

file(GLOB_RECURSE TEST_SOURCES ${WILDCARD_SOURCE})

add_executable(libtests ${TEST_SOURCES})

find_package( Catch2 REQUIRED )

# Link to the desired libraries
target_link_libraries(libtests
    PRIVATE
    Catch2::Catch2
    ${OpenCV_LIBS}
    Preprocessing
    Decoding
)

include(CTest)
include(Catch)
catch_discover_tests(libtests)