#include "LV2Adaptor.h"

using namespace RackoonIO::Hosting;

LV2Adaptor::LV2Adaptor(std::string uname) : RackUnit(uname) {

}
void LV2Adaptor::setPlatform(const LV2Platform* platform) {
	mPlatform = platform;
}

void LV2Adaptor::loadPlugin(std::string uri) {
	mPlugin = ((LV2Platform*) mPlatform)->getPlugin(uri);
	if(!mPlugin)
		return;

	auto inputs = mPlugin->getPortsOfType(LV2Port::Input);
	auto outputs = mPlugin->getPortsOfType(LV2Port::Output);

	std::vector<const LV2Port*>::const_iterator it;
	for(it = inputs.begin(); it != inputs.end(); it++) {
		addJack((*it)->symbol, JACK_SEQ);
	}

	for(it = outputs.begin(); it != outputs.end(); it++) {
		addPlug((*it)->symbol);
	}
}

bool LV2Adaptor::initPlugin() {
	if(!mPlugin)
		return false;

	return mPlugin->init();
}

void LV2Adaptor::feedPlugin(Jack *jack) {

}

