#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include "common.h"
#include "framework/factories/GenericEventFactory.h"

namespace RackoonIO {


class EventLoop {
	std::vector< std::vector< std::function< void(shared_ptr<EventMessage>) > > > eventListeners;
	int maxEventTypes;
public:
	EventLoop(int);

	void addEventListener(std::function<void(shared_ptr<EventMessage>)>);
	void triggerEvent(unique_ptr<EventPayload>);
	void cycle();
};
}
#endif
