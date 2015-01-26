#ifndef RACKUNITGENERICFACTORY_H
#define RACKUNITGENERICFACTORY_H
#include "framework/rack/RackUnit.h"
namespace RackoonIO {
class RackUnitGenericFactory {
	virtual std::unique_ptr<RackUnit> build(std::string, std::string) = 0;
};

}
#endif
