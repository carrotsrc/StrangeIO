#include "common.h"
#include "framework/events/EventLoop.h"
#include "framework/events/FrameworkMessages.h"

void cbProcComplete(std::shared_ptr<RackoonIO::EventMessage> msg) {
	std::cout << "CB1 Event Code: " << msg->msgType << endl;
}

void cbProcComplete2(std::shared_ptr<RackoonIO::EventMessage> msg) {
	std::cout << "CB2 Event Code: " << msg->msgType << endl;
}

int main( void ) {
	RackoonIO::EventLoop loop;
	RackoonIO::GenericEventMessageFactory factory;
	std::thread t;

	auto msgA = factory.createMessage(FwProcComplete);
	auto msgB = factory.createMessage(FwTestEvent);
	loop.initEvents(0);
	loop.start();
	loop.addEventListener(FwProcComplete, std::bind(&cbProcComplete, std::placeholders::_1));
	loop.addEventListener(FwTestEvent, std::bind(&cbProcComplete2, std::placeholders::_1));
	loop.addEvent(std::move(msgA));
	loop.addEvent(std::move(msgB));
	int k;
	cin >> k;
	loop.stop();
}
