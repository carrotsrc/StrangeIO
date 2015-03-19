cmake_minimum_required (VERSION 2.6)
project (librackio)

# Build
set( GCC_CFLAG "-std=c++11 -fPIC -shared" )
set( GCC_LDFLAGS "-wl,-soname,librackio.so" )

# Libs
include_directories("${PROJECT_SOURCE_DIR}/")
set( EXTRA_LIBS ${EXTRA_LIBS} asound pthread )

# Files
set( FWDIR ${CMAKE_CURRENT_SOURCE_DIR}/framework ) 
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
	${FWDIR}/rack/MidiModule.cpp
	${FWDIR}/rack/MidiRouter.cpp
	${FWDIR}/rack/UnitConnectors.cpp
	${FWDIR}/rack/RackUnit.cpp
	${FWDIR}/rack/RackChain.cpp
	${FWDIR}/rack/Rack.cpp
)

set( SRC_TELEMETRY
	${FWDIR}/telemetry/RackTelemetry.cpp
)

set( SRC_THREADS
	${FWDIR}/threads/WorkerThread.cpp
	${FWDIR}/threads/ThreadPool.cpp
	${FWDIR}/threads/RackQueue.cpp
)

set( SRC_ALL
	${SRC_BUFFERS}
	${SRC_EVENTS}
	${SRC_FACTORIES}
	${SRC_MEMORY}
	${SRC_RACK}
	${SRC_TELEMETRY}
	${SRC_THREADS}
)

# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )

add_library( librackio.so SHARED ${SRC_ALL})
target_link_libraries ( librackio.so  ${EXTRA_LIBS} )
