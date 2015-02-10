#include "EventMessageFactory.h"

using namespace RackoonIO;

std::unique_ptr<EventMessage> EventMessageFactory::createMessage(EventType type) {
	EventMessage *msg = nullptr;
	switch(type) {
	case FramesFinalBuffer:
		msg = (EventMessage*) new EvFramesFinalBuffer; 
		break;
	}

	msg->msgType = type;
	return std::unique_ptr<EventMessage>(msg);
}
