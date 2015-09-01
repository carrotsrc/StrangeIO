#include "GenericEventMessageFactory.h"
#include "framework/events/FrameworkMessages.h"

using namespace strangeio;

std::unique_ptr<EventMessage> GenericEventMessageFactory::createMessage(EventType type) {
	if(type < 1000)
		return clientMessage(type);
	else
		return frameworkMessage(type);
}

std::unique_ptr<EventMessage> GenericEventMessageFactory::frameworkMessage(EventType type) {
	EventMessage *msg = nullptr;
	switch(type) {
	case FwTestEvent:
	case FwProcComplete: msg =(EventMessage*) new MsgNotification(); break;
	}

	msg->msgType = type;

	return std::unique_ptr<EventMessage>(msg);
}

