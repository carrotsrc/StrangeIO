#include "RackUnitGenericFactory.h"
using namespace RackoonIO;

void RackUnitGenericFactory::setDependencies(RackUnit *unit) {
	unit->setEventLoop(eventLoop);
	unit->setMessageFactory(messageFactory);
}


void RackUnitGenericFactory::setEventLoop(EventLoop *loop) { 
	eventLoop = loop; 
}

void RackUnitGenericFactory::setMessageFactory(GenericEventMessageFactory *factory) { 
	messageFactory = factory; 
}
