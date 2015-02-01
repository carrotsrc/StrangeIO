#include "RuEcho.h"
using namespace RackoonIO;

RuEcho::RuEcho()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	mDelay = 50;
	feedDecay = 0.5;
	sampleRate = 44100;
	workState = IDLE;
	processedPeriod = NULL;
	remainder = false;

	MIDI_BIND("feedbackDecay", RuEcho::midiFeedbackDecay);
}

void RuEcho::writeDebugPCM(short value) {
	short *d = (short*)calloc(256, sizeof(short));
	for(int i = 0; i < 256; i++)
		d[i] = value;
	fwrite(d, sizeof(short), 256, fp);
	free(d);
}

FeedState RuEcho::feed(RackoonIO::Jack *jack) {
	short *period = NULL;
	Jack *out = getPlug("audio_out")->jack;
	int frames = jack->frames;
	out->frames = frames;

	if(workState == BYPASS)
		return out->feed(period);


	if(remainder) {
		if(out->feed(processedPeriod) == FEED_WAIT)
			return FEED_WAIT;

		remainder = false;
		processedPeriod = nullptr;
		dLevel += frames;
		return FEED_WAIT;
	}

	jack->flush(&period);

	if(workState == PRIMING) {

		if(feedbackPeriod == nullptr)
			feedbackPeriod = (short*) calloc(frames, sizeof(short));

		if(dLevel + frames > (bufSize)) {
			workState = RUNNING;
		} else {
			memcpy(fDelay+dLevel, period, (frames*sizeof(short)));

			if(out->feed(period) == FEED_OK)
				dLevel += frames;
			else
				return FEED_WAIT;
		}
	}

	if(workState == RUNNING) {
		processedPeriod = (short*)calloc(frames, sizeof(short));

		if((dLevel + frames) > (bufSize)) {
			dLevel = 0;
		}

		for(int i = 0; i < frames; i++) {
			*(processedPeriod+i) = *(period+i) + *(fDelay+dLevel+i) + *(feedbackPeriod+i);
			*(period+i) = *(processedPeriod+i) * feedDecay;
		}
		memcpy(fDelay+dLevel, period, sizeof(short)*frames);
		free(period);

		if(out->feed(processedPeriod) == FEED_WAIT) {
			remainder = true;
			return FEED_OK;
		}

		dLevel += frames;
	}


	return FEED_OK;

}

void RuEcho::add(short *period, int size) {
}

void RuEcho::setConfig(string config, string value) {
	if(config == "delay")
		mDelay = atoi(value.c_str());
	if(config == "decay")
		feedDecay = atof(value.c_str());
}

RackState RuEcho::init() {
	workState = PRIMING;
	bufSize = (((sampleRate<<1)/1000)*mDelay);
	frameBuffer = (short*) calloc(bufSize, sizeof(short));
	feedbackPeriod = nullptr;
	if(frameBuffer == NULL)
		cout << "Error" << endl;

	dLevel = 0;
	fDelay = frameBuffer;

	cout << "RuEcho: Initialised" << endl;
}

RackState RuEcho::cycle() {
	if(workState == RUNNING && remainder) {
		Jack *out = getPlug("audio_out")->jack;
	}
	return RACK_UNIT_OK;
}

void RuEcho::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}

void RuEcho::midiFeedbackDecay(int value) {
	feedDecay = ((0.78*value)/100);
}
