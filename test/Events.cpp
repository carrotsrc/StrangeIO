#include "common.h"
#include "framework/events/EventLoop.h"
#include "framework/events/FrameworkMessages.h"

void cbProcComplete(std::shared_ptr<RackoonIO::EventMessage> msg) {
	std::cout << "Event Code: " << msg->msgType << endl;
}

int main( void ) {
	RackoonIO::EventLoop loop;
	RackoonIO::GenericEventMessageFactory factory;

	auto msg = factory.createMessage(FwProcComplete);
	loop.initEvents(0);
	loop.addEventListener(FwProcComplete, std::bind(&cbProcComplete, std::placeholders::_1));
	loop.addEvent(std::move(msg));
	loop.cycle();
}
