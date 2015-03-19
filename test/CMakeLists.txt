cmake_minimum_required (VERSION 2.6)
project (RackioEx)

# Build
set( GCC_CFLAG "-std=c++11" )

# Libs
include_directories("${PROJECT_SOURCE_DIR}/../")
set( EXTRA_LIBS ${EXTRA_LIBS} -L../ -lrackio )
set( EXTRA_LIBS ${EXTRA_LIBS} asound )

# Files
# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )

add_executable( bin/tevents  Events.cpp)
add_executable( bin/ttelemetry Telemetry.cpp)
add_executable( bin/tnormalbuffer NormalisedSizeBuffer.cpp)
add_executable( bin/tdelaybuffer DelayBuffer.cpp)
add_executable( bin/tcache Cache.cpp)
target_link_libraries ( bin/tevents  ${EXTRA_LIBS} )
target_link_libraries ( bin/ttelemetry  ${EXTRA_LIBS} )
target_link_libraries ( bin/tnormalbuffer  ${EXTRA_LIBS} )
target_link_libraries ( bin/tdelaybuffer  ${EXTRA_LIBS} )
target_link_libraries ( bin/tcache  ${EXTRA_LIBS} )
