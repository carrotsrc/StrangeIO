#ifndef RACKUNITFACTORY_H
#define RACKUNITFACTORY_H
#include "framework/factories/RackUnitGenericFactory.h"
namespace RackoonIO {
class RackUnitFactory : public RackUnitGenericFactory {
public:
	std::unique_ptr<RackUnit> build(std::string, std::string);
};

}
#endif
