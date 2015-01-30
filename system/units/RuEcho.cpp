#include "RuEcho.h"
using namespace RackoonIO;

RuEcho::RuEcho()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	mDelay = 50;
	sampleRate = 44100;
	workState = IDLE;
	processedPeriod = nullptr;
	fp = fopen("echo.raw", "wb");
}

FeedState RuEcho::feed(RackoonIO::Jack *jack) {
	short *period = NULL;
	Jack *out = getPlug("audio_out")->jack;
	int frames = jack->frames;
	out->frames = frames;
	jack->flush(&period);

	if(workState == BYPASS)
		return out->feed(period);


	if(processedPeriod != nullptr) {
		if(out->feed(processedPeriod) == FEED_WAIT)
			return FEED_WAIT;

		processedPeriod = nullptr;
	}

	if(workState == PRIMING) {
		if(dLevel + frames > bufSize) {
			workState = RUNNING;
		} else {
			memcpy(fDelay, period, (frames*sizeof(short)));
			if(out->feed(period) == FEED_OK) {
				fDelay += frames;
				dLevel += frames;
			} else
				return FEED_WAIT;
		}
	}

	if(workState == RUNNING) {
		add(period, frames);
		if(out->feed(processedPeriod) == FEED_WAIT)
			return FEED_WAIT;

		free(period);
		fDelay += frames;
		dLevel += frames;
		processedPeriod = nullptr;
	}


	return FEED_OK;

}

void RuEcho::add(short *period, int size) {
	processedPeriod = (short*)malloc(sizeof(short)*size);
	if((dLevel + size) > bufSize) {
		dLevel = 0;
		fDelay = frameBuffer;
	}

	for(int i = 0; i < size; i++) {
		processedPeriod[i] = period[i] + (fDelay[i]>>2);
	}

	memcpy(fDelay, period, sizeof(short)*size);
}

void RuEcho::setConfig(string config, string value) {
	if(config == "delay")
		mDelay = atoi(value.c_str());
}

RackState RuEcho::init() {
	workState = PRIMING;
	bufSize = (((sampleRate<<1)/1000)*mDelay);
	frameBuffer = (short*) malloc(sizeof(short) * (bufSize));
	if(frameBuffer == NULL)
		cout << "Error" << endl;
	fDelay = frameBuffer;
	dLevel = 0;

	cout << "RuEcho: Initialised" << endl;
}

RackState RuEcho::cycle() {
	if(workState == RUNNING && processedPeriod != nullptr) {
		Jack *out = getPlug("audio_out")->jack;
	}
	return RACK_UNIT_OK;
}
