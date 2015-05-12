#ifndef HOSTINGLV2_H
#define HOSTINGLV2_H

#include "framework/common.h"
#include "framework/hosting/lv2/LV2Adaptor.h"
#include "framework/factories/RackUnitGenericFactory.h"

using namespace RackoonIO;

class TestAdaptor : public Hosting::LV2Adaptor {

public:
	TestAdaptor() : LV2Adaptor("TestAdaptor") {

	}
	RackState init() {
		loadPlugin("http://plugin.org.uk/swh-plugins/amp");
		return RACK_UNIT_OK;
	}

	void setConfig(std::string, std::string) {

	}

	FeedState feed(Jack *jack) {
		return FEED_OK;
	}

	RackState cycle() {
		return RACK_UNIT_OK;
	}

	void block(Jack*) {
	}

protected:
	void feedPlugin(Jack *jack) {

	}
};

class TestFactory : public RackUnitGenericFactory {
public:
	std::unique_ptr<RackUnit> build(std::string type, std::string name) {

		auto u = new TestAdaptor();
		u->setName(name);
		setDependencies(u);
		return std::unique_ptr<RackUnit>(u);
	}
};

#endif
