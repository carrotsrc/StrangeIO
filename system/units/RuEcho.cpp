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
}

FeedState RuEcho::feed(RackoonIO::Jack *jack) {
	short *period = NULL;
	Jack *out = getPlug("audio_out")->jack;
	int frames = jack->frames;
	out->frames = frames;
	jack->flush(&period);

	if(workState == BYPASS)
		return out->feed(period);


	if(remainder) {
		if(out->feed(processedPeriod) == FEED_WAIT)
			return FEED_WAIT;
		fwrite(processedPeriod, sizeof(short), frames, fp);
		remainder = false;
	}

	if(workState == PRIMING) {
		if(dLevel + frames > (bufSize)) {
			workState = RUNNING;
		} else {
			memcpy(frameBuffer+dLevel, period, (frames*sizeof(short)));
			if(out->feed(period) == FEED_OK)
				dLevel += frames;
			else
				return FEED_WAIT;
		}
	}

	if(workState == RUNNING) {
		processedPeriod = NULL;
		processedPeriod = (short*)calloc(frames, sizeof(short));

		if((dLevel + frames) > (bufSize))
			dLevel = 0;


		memcpy(processedPeriod, frameBuffer+dLevel, frames*sizeof(short));
		memcpy(frameBuffer+dLevel, period, sizeof(short)*frames);

//		add(period, frames);
		if(out->feed(processedPeriod) == FEED_WAIT) {
			return FEED_WAIT;
			remainder = true;
		}
		fwrite(processedPeriod, sizeof(short), frames, fp);
		short *d = (short*)calloc(256, sizeof(short));
		for(int i = 0; i < 256; i++)
			d[i] = -3567;
		fwrite(d, sizeof(short), 256, fp);
		free(period);

		free(d);
		fDelay += frames;
		dLevel += frames;
	}


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
	if(frameBuffer == NULL)
		cout << "Error" << endl;

	dLevel = 0;

	cout << "RuEcho: Initialised" << endl;
}

RackState RuEcho::cycle() {
	if(workState == RUNNING && processedPeriod != nullptr) {
		Jack *out = getPlug("audio_out")->jack;
	}
	return RACK_UNIT_OK;
}
