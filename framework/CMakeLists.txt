cmake_minimum_required (VERSION 2.6)
project (librackio)

# Build
set( GCC_CFLAG "-std=c++11 -fPIC -shared -ggdb -Wall" )
set( GCC_LDFLAGS "-wl,-soname,librackio.so" )
set( RACK_TELEMETRY "0" CACHE TELEMETRY "Compile telemetry code for RackoonIO::Rack" )

configure_file(
	"${PROJECT_SOURCE_DIR}/Telemetry.h.in"
	"${PROJECT_SOURCE_DIR}/Telemetry.h"
)

# Libs
include_directories("${PROJECT_SOURCE_DIR}/../")
set( EXTRA_LIBS ${EXTRA_LIBS} asound pthread )

# Files
set( FWDIR ${CMAKE_CURRENT_SOURCE_DIR} ) 
set( SRC_BUFFERS 
	${FWDIR}/buffers/DelayBuffer.cpp
	${FWDIR}/buffers/NormalisedSizeBuffer.cpp
	${FWDIR}/buffers/RingBuffer.cpp
)

set( SRC_EVENTS
	${FWDIR}/events/EventLoop.cpp
)

set( SRC_FACTORIES
	${FWDIR}/factories/GenericEventMessageFactory.cpp
	${FWDIR}/factories/RackUnitGenericFactory.cpp
)

set( SRC_MEMORY
	${FWDIR}/memory/CacheHandler.cpp
	${FWDIR}/memory/BitfieldCache.cpp
)

set( SRC_RACK
	${FWDIR}/rack/UnitConnectors.cpp
	${FWDIR}/rack/RackUnit.cpp
	${FWDIR}/rack/RackChain.cpp
	${FWDIR}/rack/Rack.cpp
)

set( SRC_MIDI
	${FWDIR}/midi/MidiModule.cpp
	${FWDIR}/midi/MidiHandler.cpp
)


set( SRC_TELEMETRY
	${FWDIR}/telemetry/RackTelemetry.cpp
)

set( SRC_THREADS
	${FWDIR}/threads/WorkerThread.cpp
	${FWDIR}/threads/ThreadPool.cpp
	${FWDIR}/threads/PackagePump.cpp
	${FWDIR}/threads/RackQueue.cpp
)

set( SRC_ALL
	${SRC_BUFFERS}
	${SRC_EVENTS}
	${SRC_FACTORIES}
	${SRC_MEMORY}
	${SRC_MIDI}
	${SRC_RACK}
	${SRC_TELEMETRY}
	${SRC_THREADS}
)

# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set( CMAKE_INSTALL_PREFIX "/usr/lib" )
add_library( rackio SHARED ${SRC_ALL})
target_link_libraries ( rackio  ${EXTRA_LIBS} )
install( FILES librackio.so DESTINATION ${CMAKE_INSTALL_PREFIX})
add_custom_command(TARGET rackio 
	POST_BUILD
	COMMAND cp librackio.so ../
	COMMENT "Copied library to repository root"
)
