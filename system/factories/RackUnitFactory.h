#ifndef RACKUNITFACTORY_H
#define RACKUNITFACTORY_H
#include "framework/factories/RackUnitGenericFactory.h"
class RackUnitFactory : public RackoonIO::RackUnitGenericFactory {
public:
	std::unique_ptr<RackoonIO::RackUnit> build(std::string, std::string);
};

#endif
