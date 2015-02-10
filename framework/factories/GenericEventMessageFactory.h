#ifndef GENERICEVENTMESSAGEFACTORY_H
#define GENERICEVENTMESSAGEFACTORY_H

#include "common.h"
#include "framework/events/EventMessage.h"

namespace RackoonIO {

class GenericEventMessageFactory {
public:
	virtual std::unique_ptr<EventMessage> createMessage(EventType) = 0;
};

}
#endif
