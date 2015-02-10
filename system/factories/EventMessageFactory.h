#ifndef EVENTMESSAGEFACTORY_H
#define EVENTMESSAGEFACTORY_H
#include "framework/factories/GenericEventMessageFactory.h"
#include "system/events/ShellEvents.h"

class EventMessageFactory 
: public RackoonIO::GenericEventMessageFactory {
	std::unique_ptr<RackoonIO::EventMessage> createMessage(RackoonIO::EventType);
};
#endif
