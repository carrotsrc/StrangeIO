#include "framework/common.h"
#include "framework/rack/RackUnit.h"
#include "framework/factories/RackUnitGenericFactory.h"
class TmpFactory : public StrangeIO::RackUnitGenericFactory {
	std::unique_ptr<StrangeIO::RackUnit> build(std::string d, std::string e) {
		return  nullptr;
	}

};

int main(void) {
	auto factory = TmpFactory();

	auto unit = factory.load("bin/dyn.so", "TestAdaptor", "TestAd1");
	unit->init();
	unit->feed(NULL);
	unit->cycle();
}
