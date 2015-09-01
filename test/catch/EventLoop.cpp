/* Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#define CATCH_CONFIG_MAIN

#include <future>
#include "catch.hpp"
#include "framework/events/EventLoop.h"
#include "framework/events/FrameworkMessages.h"
#include "framework/rack/RackUnit.h"
#include "framework/rack/config/RackAssembler.h"
#include "units/TestingUnit.h"

#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif
using namespace strangeio;
TEST_CASE( "Check Generic Message Factory", "[EventLoopFactory]" ) {
	GenericEventMessageFactory factory;
	auto msgProc = factory.createMessage(FwProcComplete);
	REQUIRE( msgProc->msgType == FwProcComplete );

	auto msgTest = factory.createMessage(FwTestEvent);
	REQUIRE( msgTest->msgType == FwTestEvent );

}

TEST_CASE( "Start and stop the event loop", "[EventLoopStart]" ) {
	EventLoop el;
	el.initEvents(0);
	el.start();

	REQUIRE( el.isRunning() == true );
	REQUIRE( el.isActive() == true );

	el.stop();

	REQUIRE( el.isRunning() == false );
	REQUIRE( el.isActive() == false );
}

static std::atomic<int> eventCode;

void event_callback(std::shared_ptr<EventMessage> msg) {
	eventCode = msg->msgType;
}

TEST_CASE( "Basic Listener", "[EventLoopEvent]" ) {
	EventLoop el;
	GenericEventMessageFactory factory;

	el.initEvents(0);
	el.start();

	el.addEventListener(FwProcComplete,std::bind(&event_callback, std::placeholders::_1));
	eventCode = 303;

	auto msg = factory.createMessage(FwProcComplete);
	el.addEvent(std::move(msg));
	std::this_thread::sleep_for(std::chrono::milliseconds(20));


	REQUIRE( eventCode == FwProcComplete );
	el.stop();
}

TEST_CASE( "Unit creates an event", "[EventLoopUnitEvent]" ) {
	EventLoop el;
	int eventCode = 303;
	GenericEventMessageFactory factory;

	Config::RackAssembler as(std::unique_ptr<RackUnitGenericFactory>(new RackUnitGenericFactory()));
	auto unit = as.assembleUnit("MainlineUnit", "mainline", "./units/MainlineUnit.rso");
	unit->setEventLoop(&el);
	unit->setMessageFactory(&factory);
	(static_cast<Testing::TestingUnit*>(unit.get()))->setCycleCheck(&eventCode);
	unit->setConfig("cycle_type", std::to_string(CYCLE_EVENTLOOP));
	unit->setConfig("create_listener", std::to_string(FwTestEvent));

	el.initEvents(0);
	el.start();

	unit->cycle();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));


	REQUIRE( eventCode == FwTestEvent );
	el.stop();
}
