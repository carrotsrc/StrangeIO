#ifndef HOSTINGLV2_H
#define HOSTINGLV2_H

#include "framework/common.h"
#include "framework/hosting/lv2/LV2Adaptor.h"
#include "framework/factories/RackUnitGenericFactory.h"

using namespace RackoonIO;

class TestAdaptor : public Hosting::LV2Adaptor {

public:
	TestAdaptor() : LV2Adaptor("TestAdaptor") {
		mGain = 2.0f;
	}
	RackState init() {
		loadPlugin("http://plugin.org.uk/swh-plugins/amp");
		initPlugin();
		mPlugin->instantiate();
		mPlugin->connectPort("Amps gain (dB)", &mGain);
		return RACK_UNIT_OK;
	}

	void setConfig(std::string, std::string) {

	}

	FeedState feed(Jack *jack) {
		PcmSample *buffer;
		out = cacheAlloc(1);
		jack->flush(&buffer);

		std::cout << "TestAdaptor" << std::endl << "--------" << std::endl;

		for(int i = 0; i < 16; i++) {
			std::cout << buffer[i] << std::endl;
		}

		mPlugin->connectPort("Input", buffer);
		mPlugin->connectPort("Output", out);
		mPlugin->run(jack->frames);
		cacheFree(buffer);
		getPlug("Output")->jack->feed(out);
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

	PcmSample *out;
	float mGain;
};

class Terminal : public RackUnit {
	public:

	Terminal() : RackUnit("Terminal") {
		addJack("audio_in", JACK_SEQ);
	}

	RackState init() {
		return RACK_UNIT_OK;
	}

	void setConfig(std::string, std::string) {

	}

	FeedState feed(Jack *jack) {
		PcmSample *samples;
		jack->flush(&samples);
		std::cout << "Terminal" << std::endl << "--------" << std::endl;

		for(int i = 0; i < 16; i++) {
			std::cout << samples[i] << std::endl;
		}
		cacheFree(samples);
		return FEED_OK;
	}

	RackState cycle() {
		return RACK_UNIT_OK;
	}

	void block(Jack*) {
	}
};

class TestFactory : public RackUnitGenericFactory {
public:
	std::unique_ptr<RackUnit> build(std::string type, std::string name) {
		RackUnit *u = nullptr;

		if(type == "TestAdaptor")
			u = dynamic_cast<RackUnit*>(new TestAdaptor());
		else
			u = dynamic_cast<RackUnit*>(new Terminal());

		u->setName(name);
		setDependencies(u);
		return std::unique_ptr<RackUnit>(u);
	}
};

#endif
