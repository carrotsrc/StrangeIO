#include "LV2Adaptor.h"

using namespace RackoonIO::Hosting;

LV2Adaptor::LV2Adaptor() {

}

void LV2Adaptor::setPlatform(const LV2Platform* platform) {
	mPlatform = platform;
}

void LV2Adaptor::loadPlugin(std::string uri) {
	mPlugin = ((LV2Platform*) mPlatform)->getPlugin(uri);
	if(!mPlugin)
		return;
}

bool LV2Adaptor::init() {
	if(!mPlugin)
		return false;

	return mPlugin->init();
}


