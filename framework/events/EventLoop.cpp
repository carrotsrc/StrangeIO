#include "EventLoop.h"

using namespace RackoonIO;

EventLoop::EventLoop() {
}

void EventLoop::initEvents(int numEvents) {
	while(numEvents-- > 0)
		eventListeners.push_back(std::vector< std::function< void(shared_ptr<EventMessage>) > >());
}

void EventLoop::addEventListener(EventType event, std::function<void(shared_ptr<EventMessage>)> callback) {
	eventListeners[event].push_back(callback);
}

void EventLoop::triggerEvent(unique_ptr<EventMessage> msg) {
	cout << "Recieved Message of type " << msg->msgType << endl;
	std::shared_ptr<EventMessage> sharedMsg(std::move(msg));

	if(sharedMsg->msgType >= eventListeners.size())
		return;

	std::vector< std::function< void(shared_ptr<EventMessage>) > >::iterator it;
	for(it = eventListeners[sharedMsg->msgType].begin();
	    it != eventListeners[sharedMsg->msgType].end();
	    ++it) {
		(*it)(sharedMsg);
	}
}
