#ifndef RACKUNITGENERICFACTORY_H
#define RACKUNITGENERICFACTORY_H
#include "framework/rack/RackUnit.h"
#include "framework/factories/GenericEventMessageFactory.h"
namespace RackoonIO {
class RackUnitGenericFactory {
protected:
	EventLoop *eventLoop;
	GenericEventMessageFactory *messageFactory;
	void setDependencies(RackUnit *unit);

public:
	virtual std::unique_ptr<RackUnit> build(std::string, std::string) = 0;
	void setEventLoop(EventLoop*);
	void setMessageFactory(GenericEventMessageFactory*);

};

}
#endif
