cmake_minimum_required (VERSION 2.6)
project (libstrangeio)

# Build
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -shared -Wall" )
set( GCC_LDFLAGS "-wl,-soname,libstrangeio.so" )

set( DEVBUILD "0" CACHE DEVBUILD "Expose development and testing code" )
set( CACHE_TRACKING "0" CACHE CACHE_TRACKING "Enable ability to track cache usage" )
set( ALSA_MIDI "1" CACHE ALSA_MIDI "User ALSA as midi driver interface" )

set( HOSTING_LV2 "0" CACHE LV2HOST "Include LV2 host platform" )

configure_file(
	"${PROJECT_SOURCE_DIR}/common.h.in"
	"${PROJECT_SOURCE_DIR}/common.h"
)

configure_file(
	"${PROJECT_SOURCE_DIR}/include/framework/fwcommon.hpp.in"
	"${PROJECT_SOURCE_DIR}/include/framework/fwcommon.hpp"
)

set( DEBUG_BUILD $ENV{DEBUG_BUILD} )

if(DEBUG_BUILD)
	MESSAGE("Debug Build")
	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g" )
endif(DEBUG_BUILD)

# Libs
include_directories("${PROJECT_SOURCE_DIR}/../" "${PROJECT_SOURCE_DIR}/include/")
set( EXTRA_LIBS ${EXTRA_LIBS} asound pthread )

# Files
set( FWDIR ${CMAKE_CURRENT_SOURCE_DIR} ) 
set( SRC_BUFFERS 
	${FWDIR}/buffer/delay.cpp
        ${FWDIR}/buffer/circular.cpp
)

set( SRC_EVENTS
	${FWDIR}/events/loop.cpp
	${FWDIR}/events/event_utility.cpp
)

set( SRC_MEMORY
	${FWDIR}/memory/cache_manager.cpp
	${FWDIR}/memory/cache_ptr.cpp
	${FWDIR}/memory/cptr_utility.cpp
)

set( SRC_CONFIG
	${FWDIR}/config/document.cpp
	${FWDIR}/config/assembler.cpp
)


set( SRC_MIDI
        ${FWDIR}/midi/led_toggle_utility.cpp
	${FWDIR}/midi/input_handle.cpp
        ${FWDIR}/midi/output_handle.cpp	
	${FWDIR}/midi/device.cpp
	${FWDIR}/midi/midi_handler.cpp
)

if(ALSA_MIDI)
set( SRC_MIDI
	${SRC_MIDI}
	${FWDIR}/midi/alsa_input.cpp
        ${FWDIR}/midi/alsa_output.cpp
	${FWDIR}/midi/alsa_driver_utility.cpp	
)
endif(ALSA_MIDI)


set( SRC_THREADS
	${FWDIR}/threads/worker.cpp
	${FWDIR}/threads/pool.cpp
	${FWDIR}/threads/pump.cpp
	${FWDIR}/threads/queue.cpp
	${FWDIR}/threads/task_utility.cpp
        ${FWDIR}/threads/scheduled.cpp
)

set( SRC_ROUTINE
	${FWDIR}/routine/sound.cpp
	${FWDIR}/routine/midi.cpp
	${FWDIR}/routine/system.cpp
)

set( SRC_COMPONENT
        ${FWDIR}/component/registrable.cpp
        ${FWDIR}/component/registry.cpp
	${FWDIR}/component/linkable.cpp
	${FWDIR}/component/unit.cpp
        ${FWDIR}/component/mount.cpp
        ${FWDIR}/component/backend.cpp
        ${FWDIR}/component/utility_container.cpp
	${FWDIR}/component/rack.cpp

        ${FWDIR}/component/unit_loader.cpp
)

set( SRC_SPECS 
	${FWDIR}/spec/mainline.cpp
	${FWDIR}/spec/combine.cpp
	${FWDIR}/spec/dispatch.cpp
)

set( SRC_HOSTING "")
if(HOSTING_LV2)
#	set( EXTRA_LIBS ${EXTRA_LIBS} lilv-0 dl sratom-0 sord-0 serd-0 )
#	include_directories(
#		"/usr/include/lilv-0"
#		"/usr/include/sratom-0"
#		"/usr/include/sord-0"
#		"/usr/include/serd-0"
#	)

#	set( SRC_HOSTING
#		${SRC_HOSTING}
#		${FWDIR}/hosting/lv2/LV2Node.cpp
#		${FWDIR}/hosting/lv2/LV2NodeFactory.cpp
#		${FWDIR}/hosting/lv2/LV2Plugin.cpp
#		${FWDIR}/hosting/lv2/LV2Platform.cpp
#		${FWDIR}/hosting/lv2/LV2Adaptor.cpp
#	)

endif(HOSTING_LV2)

set( SRC_ALL
	${SRC_ROUTINE}
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
	${SRC_SPECS}
)


# Integration
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_FLAGS}" )
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set( CMAKE_INSTALL_PREFIX "/usr/lib" )

add_library( strangeio SHARED ${SRC_ALL})
target_link_libraries( strangeio ${EXTRA_LIBS})

install( FILES libstrangeio.so DESTINATION ${CMAKE_INSTALL_PREFIX})

add_custom_command(TARGET strangeio
	POST_BUILD
	COMMAND cp libstrangeio.so ../
	COMMENT "Copied library to repository root"
)
