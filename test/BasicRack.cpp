#include "framework/events/EventLoop.h"
#include "framework/events/FrameworkMessages.h"
#include "framework/rack/Rack.h"
void onCycleEvent(std::shared_ptr<RackoonIO::EventMessage> msg) {
	std::cout << "Local callback" << std::endl;
}

int main( void ) {
	RackoonIO::EventLoop *loop;
	RackoonIO::GenericEventMessageFactory factory;
	RackoonIO::Rack rack;
	auto msgA = factory.createMessage(FwProcComplete);
	
	rack.initEvents(0);
	loop = rack.getEventLoop();
	rack.start();
	bool running = false;

	while(!running)
		running = loop->isRunning();

	loop->addEventListener(FwProcComplete, std::bind(&onCycleEvent, std::placeholders::_1));
	loop->addEvent(std::move(msgA));

	int k;
	std::cin >> k;
	loop->stop();
}
