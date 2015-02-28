cmake_minimum_required (VERSION 2.6)
project (RackioEx)

# Build
set( GCC_CFLAG "-std=c++11" )

# Libs
include_directories("${PROJECT_SOURCE_DIR}/../")
set( EXTRA_LIBS ${EXTRA_LIBS} -L../ -lrackio )
set( EXTRA_LIBS ${EXTRA_LIBS} asound )

# Files
set( UNIT_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/src/RuAlsa.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/RuImpulse.cpp 
)

set( SYS_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/src/RackUnitFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Entry.cpp 
)
# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )

add_executable( rackioex ${UNIT_SRC} ${SYS_SRC} )
target_link_libraries ( rackioex  ${EXTRA_LIBS} )
