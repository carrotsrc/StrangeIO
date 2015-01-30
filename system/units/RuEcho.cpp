#include "RuEcho.h"
using namespace RackoonIO;

RuEcho::RuEcho()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	mDelay = 50;
	sampleRate = 44100;
	workState = IDLE;
}

FeedState RuEcho::feed(RackoonIO::Jack *jack) {
	short *period;
	Jack *out = getPlug("audio_out")->jack;
	if(workState == BYPASS) {
		if(jack->flush(&period) == FEED_OK) {
			Jack *out = getPlug("audio_out")->jack;
			out->frames = jack->frames;
			return out->feed(period);
		}
		return FEED_OK;
	}

	if(workState == PRIMING) {
		if(jack->flush(&period) == FEED_OK) {
			if(fLevel + jack->frames > bufSize) {
				workState = RUNNING;
				cout << "Switched to RUNNING" << endl;
				out->frames = jack->frames;
				fPosition = frameBuffer;
				fLevel = 0;
				return out->feed(add(period, out->frames));
			} else {
				memcpy(fPosition, period, (jack->frames*sizeof(short)));
				fPosition += jack->frames;
				fLevel += jack->frames;
				out->frames = jack->frames;
				return out->feed(period);
			}
		}
	}

	if(workState == RUNNING) {
		if(jack->flush(&period) == FEED_OK) {
			out->frames = jack->frames;
			return out->feed(period);
		}
	}

}

short *RuEcho::add(short *period, int size) {
	short *addedPeriod = (short*)malloc(sizeof(short)*size);
	if(dLevel + size > bufSize) {
		fLevel = dLevel = 0;
		fPosition = fDelay = frameBuffer;
	}

	for(int i = 0; i < size; i++)
		addedPeriod[i] = period[i] + fDelay[i];

	fDelay += size;
	dLevel += size;
	memcpy(fPosition, period, sizeof(short)*size);

	fPosition += size;
	fLevel += size;

	free(period);

	return addedPeriod;
}

void RuEcho::setConfig(string config, string value) {
	if(config == "delay")
		mDelay = atoi(value.c_str());
}

RackState RuEcho::init() {
	workState = PRIMING;
	bufSize = ((sampleRate/1000)*mDelay);
	frameBuffer = (short*) malloc(sizeof(short) * (bufSize));
	fPosition = frameBuffer;
	fDelay = frameBuffer;
	dLevel = fLevel = 0;

	cout << "RuEcho: Initialised" << endl;
}

RackState RuEcho::cycle() {

}
