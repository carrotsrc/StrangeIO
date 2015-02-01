#include "RuLevels.h"

using namespace RackoonIO;
RuLevels::RuLevels()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	masterGain = 0.5;
	processed = false;
}

void RuLevels::writeDebugPCM(short value) {
	short *d = (short*)calloc(256, sizeof(short));
	for(int i = 0; i < 256; i++)
		d[i] = value;
	fwrite(d, sizeof(short), 256, fp);
	free(d);
}

FeedState RuLevels::feed(Jack *jack) {
	short *period = NULL;
	Jack *out = getPlug("audio_out")->jack;
	out->frames = jack->frames;
	jack->flush(&period);
	FeedState fState;

	if( !processed ) {
		for(int i = 0; i < jack->frames; i++)
			period[i] = period[i] * masterGain;

		processed = true;
	}

	if((fState = out->feed(period)) == FEED_OK)
		processed = false;

	return fState;
}
RackState RuLevels::init() {
	workState = READY;
	cout << "RuLevels: Initialised" << endl;
	return RACK_UNIT_OK;
}

RackState RuLevels::cycle() {
	return RACK_UNIT_OK;
}

void RuLevels::setConfig(string config, string value) {

}
