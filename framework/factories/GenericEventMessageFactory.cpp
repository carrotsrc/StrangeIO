#include "GenericEventMessageFactory.h"
#include "FrameworkMessages.h"

using namespace RackoonIO;

std::unique_ptr<EventMessage> GenericEventMessageFactory::createMessage(EventType type) {
	if(type < 1000)
		return clientMessage();
	else
		return frameworkMessage();
}

std::unique_ptr<EventMessage> GenericEventMessageFactory::frameworkMessage(EventType type) {

}
