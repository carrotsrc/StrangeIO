#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include "common.h"
#include "framework/factories/GenericEventMessageFactory.h"

namespace RackoonIO {


class EventLoop {
	std::vector< std::vector< std::function< void(shared_ptr<EventMessage>) > > > eventListeners;
	std::vector< std::unique_ptr<EventMessage> > eventQueue;
	int maxEventTypes;

	std::mutex evLock;

	void distributeMessage(std::unique_ptr<EventMessage>);
public:
	EventLoop();

	void addEventListener(EventType, std::function<void(shared_ptr<EventMessage>)>);
	void addEvent(std::unique_ptr<EventMessage>);
	void cycle();

	void initEvents(int);
};
}
#endif
