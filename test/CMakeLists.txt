cmake_minimum_required (VERSION 2.6)
project (RackioEx)

set( HOSTING_LV2 "0" CACHE LV2HOST "Include LV2 host platform" )

# Build
set( GCC_CFLAG "-std=c++11 -ggdb -Wall" )

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
add_executable( bin/tbasicrack BasicRack.cpp)
add_executable( bin/tcache Cache.cpp)
add_executable( bin/tpool ThreadPool.cpp)
add_executable( bin/tdynamic DynamicLoader.cpp)

if(HOSTING_LV2)
	include_directories(
		/usr/include/lilv-0 
		/usr/include/sratom-0
		/usr/include/sord-0 
		/usr/include/serd-0
	)
	add_executable( bin/thostinglv2  HostingLv2.cpp)
endif(HOSTING_LV2)

target_link_libraries ( bin/tevents  ${EXTRA_LIBS} )
target_link_libraries ( bin/ttelemetry  ${EXTRA_LIBS} )
target_link_libraries ( bin/tnormalbuffer  ${EXTRA_LIBS} )
target_link_libraries ( bin/tdelaybuffer  ${EXTRA_LIBS} )
target_link_libraries ( bin/tbasicrack  ${EXTRA_LIBS} )
target_link_libraries ( bin/tcache  ${EXTRA_LIBS} )
target_link_libraries ( bin/tpool  ${EXTRA_LIBS} )
target_link_libraries ( bin/tdynamic  ${EXTRA_LIBS} )

if(HOSTING_LV2)
	set( LV2LIBS 
		lilv-0
		dl
		sratom-0
		sord-0
		serd-0
	)
	target_link_libraries ( bin/thostinglv2 ${EXTRA_LIBS} ${LV2LIBS}  )
endif(HOSTING_LV2)
