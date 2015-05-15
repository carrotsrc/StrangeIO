cmake_minimum_required (VERSION 2.6)
project (RackioEx)

set( HOSTING_LV2 "0" CACHE LV2HOST "Include LV2 host platform" )

# Build
set( GCC_CFLAG "-std=c++11 -Wall" )

# Libs
include_directories("${PROJECT_SOURCE_DIR}/../")

if(HOSTING_LV2)
	include_directories(
		/usr/include/lilv-0 
		/usr/include/sratom-0
		/usr/include/sord-0 
		/usr/include/serd-0
	)
endif(HOSTING_LV2)

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

if(HOSTING_LV2)
	set( EXTRA_LIBS
		${EXTRA_LIBS}
		lilv-0
		dl
		sratom-0
		sord-0
		serd-0
	)
endif(HOSTING_LV2)
# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )

add_executable( rackioex ${UNIT_SRC} ${SYS_SRC} )
target_link_libraries ( rackioex  ${EXTRA_LIBS} )

add_custom_command(TARGET rackioex
	POST_BUILD
	COMMAND RACKOONIOFW=../ ../scripts/unitbuild RuSine ./src/RuSine.cpp
)
