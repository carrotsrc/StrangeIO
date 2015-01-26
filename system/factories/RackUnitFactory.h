#ifndef RACKUNITFACTORY_H
#define RACKUNITFACTORY_H
#include "framework/factories/RackUnitGenericFactory.h"
namespace RackoonIO {
class RackUnitFactory : public RackUnitGenericFactory {
	std::unique_ptr<RackUnit> build(std::string, std::string);
};

}
#endif
