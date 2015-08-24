cmake_minimum_required (VERSION 2.6)
project (libstrangeio)

# Build
set( GCC_CFLAG "-std=c++11 -fPIC -shared -Wall" )
set( GCC_LDFLAGS "-wl,-soname,libstrangeio.so" )

set( DEVBUILD "0" CACHE DEVBUILD "Expose development and testing code" )
set( RACK_TELEMETRY "0" CACHE TELEMETRY "Compile telemetry code for StrangeIO::Rack" )
set( HOSTING_LV2 "0" CACHE LV2HOST "Include LV2 host platform" )

configure_file(
	"${PROJECT_SOURCE_DIR}/Telemetry.h.in"
	"${PROJECT_SOURCE_DIR}/Telemetry.h"
)

configure_file(
	"${PROJECT_SOURCE_DIR}/hosting/PlatformHosting.h.in"
	"${PROJECT_SOURCE_DIR}/hosting/PlatformHosting.h"
)

configure_file(
	"${PROJECT_SOURCE_DIR}/common.h.in"
	"${PROJECT_SOURCE_DIR}/common.h"
)

configure_file(
	"${PROJECT_SOURCE_DIR}/include/framework/fwcommon.hpp.in"
	"${PROJECT_SOURCE_DIR}/include/framework/fwcommon.hpp"
)

# Libs
include_directories("${PROJECT_SOURCE_DIR}/../" "${PROJECT_SOURCE_DIR}/include/")
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

set( SRC_CONFIG
	${FWDIR}/rack/config/RackDocument.cpp
	${FWDIR}/rack/config/RackAssembler.cpp
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

set( SRC_DYNLIB
	${FWDIR}/dynlib/LibraryLoader.cpp
)

set( SRC_HELPERS
	${FWDIR}/helpers/sound.cpp
	${FWDIR}/helpers/midi.cpp
)

set( SRC_COMPONENT
	${FWDIR}/component/Unit.cpp
)

set( SRC_HOSTING "")
if(HOSTING_LV2)
	set( EXTRA_LIBS ${EXTRA_LIBS} lilv-0 dl sratom-0 sord-0 serd-0 )
	include_directories(
		"/usr/include/lilv-0"
		"/usr/include/sratom-0"
		"/usr/include/sord-0"
		"/usr/include/serd-0"
	)

	set( SRC_HOSTING
		${SRC_HOSTING}
		${FWDIR}/hosting/lv2/LV2Node.cpp
		${FWDIR}/hosting/lv2/LV2NodeFactory.cpp
		${FWDIR}/hosting/lv2/LV2Plugin.cpp
		${FWDIR}/hosting/lv2/LV2Platform.cpp
		${FWDIR}/hosting/lv2/LV2Adaptor.cpp
	)

endif(HOSTING_LV2)

set( SRC_ALL
	${SRC_HELPERS}
	${SRC_BUFFERS}
	${SRC_EVENTS}
	${SRC_FACTORIES}
	${SRC_MEMORY}
	${SRC_MIDI}
	${SRC_DYNLIB}
	${SRC_CONFIG}
	${SRC_COMPONENT}
	${SRC_RACK}
	${SRC_TELEMETRY}
	${SRC_THREADS}
	${SRC_HOSTING}
)

if(DEVBUILD)
	set( GCC_CFLAG "${GCC_CFLAG} -ggdb" )
endif(DEVBUILD)

# Integration
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CFLAG}" )
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set( CMAKE_INSTALL_PREFIX "/usr/lib" )
add_library( strangeio SHARED ${SRC_ALL})
target_link_libraries ( strangeio  ${EXTRA_LIBS} )
install( FILES libstrangeio.so DESTINATION ${CMAKE_INSTALL_PREFIX})

add_custom_command(TARGET strangeio
	POST_BUILD
	COMMAND cp libstrangeio.so ../
	COMMENT "Copied library to repository root"
)
