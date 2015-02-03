#include "RuPitchBender.h"

using namespace RackoonIO;

RuPitchBender::RuPitchBender()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
	bend = 5;
	bentPeriod = nullptr;
	oldBend = 0;
}


FeedState RuPitchBender::feed(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;

	if(bentPeriod != nullptr && out->feed(bentPeriod) == FEED_WAIT)
		return FEED_WAIT;

	bentPeriod = nullptr;

	short *period;
	int k;
	jack->flush(&period);
	if(oldBend != bend) {
		inc = (((float)jack->frames/100)*bend)<<1;
		step = jack->frames/inc;
		nsize = jack->frames+inc;
		oldBend = bend;
	}

	k = 0;
	bentPeriod = (short*)malloc(sizeof(short)*nsize);
	for(int i = 0; i < jack->frames;) {
		if(i > 0 && i%step == 0) {
			bentPeriod[k++] = (period[i]>>1);
			if(period[i] < period[i]+1) // up hill
				bentPeriod[k++] = period[i] + ((period[i+1] - period[i])>>1);
			else
			if(period[i] > period[i]+1) // down hill
				bentPeriod[k++] = period[i] - (period[i] - period[i+1]>>1);
			else
				bentPeriod[k++] = period[i];


			continue;
		}
		bentPeriod[k++] = (period[i]>>1);
	}

	free(period);

	out->frames = nsize;
	if(out->feed(bentPeriod) == FEED_WAIT)
		return FEED_OK;
	bentPeriod = nullptr;
	return FEED_OK;
}

RackState RuPitchBender::init() {
	workState = READY;
	cout << "RuPitchBender: Initialised" << endl;
	return RACK_UNIT_OK;
}

RackState RuPitchBender::cycle() {
	return RACK_UNIT_OK;
}

void RuPitchBender::setConfig(string config, string value) {

}

void RuPitchBender::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}
