cmake_minimum_required (VERSION 2.6)
project (RackioEx)

# Build
set( GCC_CFLAG "-std=c++11" )

# Libs
include_directories("${PROJECT_SOURCE_DIR}/../")
set( EXTRA_LIBS ${EXTRA_LIBS} -L../ -lrackio )
set( EXTRA_LIBS ${EXTRA_LIBS} asound )

# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )

add_executable( rackioex src/RuAlsa.cpp src/RackUnitFactory.cpp src/Entry.cpp )
target_link_libraries ( rackioex  ${EXTRA_LIBS} )
