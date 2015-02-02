#include "RuChannelMixer.h"

using namespace RackoonIO;
RuChannelMixer::RuChannelMixer()
: RackUnit() {
	addJack("channel_1", JACK_SEQ);
	addJack("channel_2", JACK_SEQ);
	addPlug("audio_out");
	mixedPeriod = periodC1 = periodC2 = nullptr;
	gainC1 = gainC2 = 1.0;
}


FeedState RuChannelMixer::feed(Jack *jack) {
	short *period;
	Jack *out = getPlug("audio_out")->jack;
	out->frames = jack->frames;

	if(mixedPeriod != nullptr) {
		if(out->feed(mixedPeriod) == FEED_WAIT)
			return FEED_WAIT;
		mixedPeriod = nullptr;
	}


	if(jack->name == "channel_1") {
		if(periodC1 != nullptr)
			return FEED_WAIT;

		jack->flush(&periodC1);
	} else {
		if(periodC2 != nullptr)
			return FEED_WAIT;

		jack->flush(&periodC2);
	}

	if(periodC1 == nullptr || periodC2 == nullptr)
		return FEED_OK;

	mixedPeriod = (short*) malloc(sizeof(short)*jack->frames);
	for(int i = 0; i < jack->frames; i++) {
		mixedPeriod[i] = ((periodC1[i]) * gainC1) +
				 ((periodC2[i]) * gainC2);
	}

	free(periodC1);
	free(periodC2);

	periodC1 = periodC2 = nullptr;
	
	if(jack->feed(mixedPeriod) == FEED_WAIT)
		return FEED_WAIT;

	mixedPeriod = nullptr;

	return FEED_OK;
}
RackState RuChannelMixer::init() {
	workState = READY;
	cout << "RuChannelMixer: Initialised" << endl;
	return RACK_UNIT_OK;
}

RackState RuChannelMixer::cycle() {
	return RACK_UNIT_OK;
}

void RuChannelMixer::setConfig(string config, string value) {

}

void RuChannelMixer::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}
