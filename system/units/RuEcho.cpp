#include "RuEcho.h"
using namespace RackoonIO;

RuEcho::RuEcho()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	mDelay = 50;
	sampleRate = 44100;
	workState = IDLE;
	processedPeriod = NULL;
	fp = fopen("echo.raw", "wb");
	remainder = false;
	x = 0;
}

void RuEcho::writeDebugPCM(short value) {
	short *d = (short*)calloc(256, sizeof(short));
	for(int i = 0; i < 256; i++)
		d[i] = value;
	fwrite(d, sizeof(short), 256, fp);
	free(d);
}

FeedState RuEcho::feed(RackoonIO::Jack *jack) {
	short *period = NULL, *procPeriod = NULL;
	Jack *out = getPlug("audio_out")->jack;
	int frames = jack->frames;
	out->frames = frames;
	//rewind(fp);
	//fwrite(frameBuffer, sizeof(short), bufSize, fp);

	/*if(workState == BYPASS)
		return out->feed(period);*/


	if(remainder) {
		procPeriod = processedPeriod;
		if(out->feed(procPeriod) == FEED_WAIT)
			return FEED_WAIT;

		remainder = false;
		processedPeriod = NULL;
		dLevel += frames;
		return FEED_WAIT;
	}

	jack->flush(&period);

	if(workState == PRIMING) {
		if(dLevel + frames > (bufSize)) {
			workState = RUNNING;
		} else {
			memcpy(fDelay+dLevel, period, (frames*sizeof(short)));
			if(out->feed(period) == FEED_OK) {
				dLevel += frames;
			} else {
				return FEED_WAIT;
			}
		}
	}

	if(workState == RUNNING) {
		procPeriod = (short*)calloc(frames, sizeof(short));

		if((dLevel + frames) > (bufSize)) {
			dLevel = 0;
		}

		memcpy(procPeriod, fDelay+dLevel, frames*sizeof(short));
		memcpy(fDelay+dLevel, period, sizeof(short)*frames);
		free(period);

		if(out->feed(procPeriod) == FEED_WAIT) {
			remainder = true;
			processedPeriod = procPeriod;
			return FEED_OK;
		}

		dLevel += frames;
	}

	rewind(fp);
	fwrite(frameBuffer, sizeof(short), bufSize, fp);

	return FEED_OK;

}

void RuEcho::add(short *period, int size) {
}

void RuEcho::setConfig(string config, string value) {
	if(config == "delay")
		mDelay = atoi(value.c_str());
}

RackState RuEcho::init() {
	workState = PRIMING;
	bufSize = (((sampleRate<<1)/1000)*mDelay);
	frameBuffer = (short*) calloc(bufSize, sizeof(short));
	f2 = (short*) calloc(bufSize, sizeof(short));
	if(frameBuffer == NULL)
		cout << "Error" << endl;

	dLevel = 0;
	fDelay = frameBuffer;
	fd = f2;

	cout << "RuEcho: Initialised" << endl;
}

RackState RuEcho::cycle() {
	if(workState == RUNNING && remainder) {
		Jack *out = getPlug("audio_out")->jack;
	}
	return RACK_UNIT_OK;
}
