#include "MainlineUnit.h"

using namespace StrangeIO;
using namespace StrangeIO::Testing;

MainlineUnit::MainlineUnit()
: TestingUnit(std::string("MainlineUnit")) {
	workState = IDLE;
	addPlug("audio_out");
	addJack("power", JACK_AC, 1);
	MidiExport("exported", MainlineUnit::exportedMethod);
	mFeed = nullptr;
}

FeedState MainlineUnit::feed(Jack *jack) {
	if(mFeed == nullptr)
		return FEED_WAIT;

	(*mFeed)++;
	PcmSample* period;
	jack->flush(&period,1);
	auto out = getPlug("audio_out")->jack;

	auto val = static_cast<int>(*period);
	switch(val) {

	case SINGLE_CHANNEL:
		return FEED_OK;
		break;

	case DOUBLE_CHANNEL:
		jack->flush(&period,2);

		if(*period != CHANNEL_TWO) {
			return FEED_WAIT;
		}

		(*mFeed)++;
		return FEED_OK;
		break;

	case FEED_TEST:
		out->numSamples = jack->numSamples;
		out->numChannels = jack->numChannels;
		return out->feed(period);
		break;

	default:
		return FEED_WAIT;
	}

	return FEED_WAIT;
}
void MainlineUnit::setConfig(std::string config, std::string value) {
}

RackState MainlineUnit::init() {
	workState = READY;
	return RACK_UNIT_OK;
}

RackState MainlineUnit::cycle() {
	(*mCycle)++;
	auto period = new PcmSample[1];
	period[0] = {CYCLE_TEST};
	auto out = getPlug("audio_out")->jack;
	out->numChannels = 1;
	out->numSamples = 1;
	out->feed(period);
	return RACK_UNIT_OK;
}

void MainlineUnit::block(Jack *jack) {
}

void MainlineUnit::exportedMethod(int value) {

}

DynamicBuilder(MainlineUnit);
