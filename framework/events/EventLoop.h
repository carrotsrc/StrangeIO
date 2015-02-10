#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include "common.h"
#include "framework/factories/GenericEventMessageFactory.h"

namespace RackoonIO {


class EventLoop {
	std::vector< std::vector< std::function< void(shared_ptr<EventMessage>) > > > eventListeners;
	int maxEventTypes;
public:
	EventLoop();

	void addEventListener(EventType, std::function<void(shared_ptr<EventMessage>)>);
	void triggerEvent(unique_ptr<EventMessage>);
	void cycle();

	void initEvents(int);
};
}
#endif
