#include "RuLoop.h"

using namespace RackoonIO;
#include "system/events/ShellEvents.h"

RuLoop::RuLoop()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
	sampleRate = 44100;
	loopBuffer = nullptr;
	MIDI_BIND("toggleRecord", RuLoop::midiToggleRecord);
	MIDI_BIND("toggleLoop", RuLoop::midiToggleLoop);
}

void RuLoop::bufferRealloc() {
	// add another second
	short *tmp = NULL;
	int offset = writePos - loopBuffer;
	while(tmp == NULL)
		tmp = (short*) realloc(loopBuffer, sizeof(short) * (loopCapacity + (sampleRate<<1)));

	loopBuffer = tmp;
	writePos = loopBuffer + offset;
	loopLength = writePos;
	loopCapacity = loopCapacity + (sampleRate<<1);
	cout << "Reallocated buffer to " << loopCapacity << endl;
}

void RuLoop::feedLoop() {
	int bufSize = frames;
	if(readPos + frames > loopLength)
		bufSize = (loopLength - readPos);

	short *period = (short*)malloc(frames*sizeof(short));
	memcpy(period, readPos, bufSize*sizeof(short));

	if(bufSize < frames) {
		readPos = loopBuffer;
		memcpy(period+bufSize, readPos, (frames-bufSize)*sizeof(short));
		readPos += (frames-bufSize);
	}
	Jack *out = getPlug("audio_out")->jack;
	CONSOLE_MSG("RuLoop", "Length: " << loopLength-loopBuffer);
	while(workState == LOOPING) {

		if(out->feed(period) == FEED_OK) {
			readPos += bufSize;
			bufSize = frames;
			if(readPos + frames > loopLength)
				bufSize = (loopLength - readPos);

			period = (short*)malloc(frames*sizeof(short));
			memcpy(period, readPos, bufSize*sizeof(short));

			if(bufSize < frames) {
				readPos = loopBuffer;
				memcpy(period+bufSize, readPos, (frames-bufSize)*sizeof(short));
				readPos += (frames-bufSize);
			}
		}
	}
}

FeedState RuLoop::feed(Jack *jack) {
	if(workState == LOOPING)
		return FEED_WAIT;

	short *period = NULL;
	Jack *out = getPlug("audio_out")->jack;
	jack->flush(&period);
	out->frames = jack->frames;
	frames = jack->frames;

	return out->feed(period);
	return FEED_OK;
}

RackState RuLoop::init() {
	workState = PASSTHROUGH;
	EVENT_LISTENER(FramesFinalBuffer, RuLoop::eventFinalBuffer);
	cout << "RuLoop: Initialised" << endl;
	return RACK_UNIT_OK;
}

RackState RuLoop::cycle() {
	return RACK_UNIT_OK;
}

void RuLoop::setConfig(string config, string value) {

}

void RuLoop::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}

void RuLoop::midiToggleRecord(int value) {
	if(value < 127)
		return;

	if(workState == READY || workState == PASSTHROUGH) {
		if(loopBuffer == nullptr) {
			// start with a two second loop (sampleRate * 2 channels * 2 seconds)
			loopBuffer = (short*) malloc( (sampleRate<<2) * sizeof(short));
			loopCapacity = sampleRate<<2;
			loopLength = readPos = writePos = loopBuffer;
		} else {
			readPos = writePos = loopBuffer;
		}
		workState = PRIMING;
		CONSOLE_MSG("RuLoop", "PRIMING");
	}
	else
	if(workState == PRIMING) {
		workState = READY;
		CONSOLE_MSG("RuLoop", "Primed and READY");
	}
}

void RuLoop::midiToggleLoop(int value) {
	if(value < 127)
		return;

	if(workState == READY) {
		workState = LOOPING;
		CONSOLE_MSG("RuLoop", "LOOPING");
	}
	else if(workState == LOOPING) {
		workState = READY;
		CONSOLE_MSG("RuLoop", "READY");
	} else
	if(workState == PRIMING) {
		workState = LOOPING;
		CONSOLE_MSG("RuLoop", "Straight into READY");
		OUTSRC(RuLoop::feedLoop);
	}
}

void RuLoop::eventFinalBuffer(std::shared_ptr<EventMessage> msg) {
	if(workState == PRIMING) {
		if( ((loopLength-loopBuffer) + FFB(msg)->numFrames) > loopCapacity)
			bufferRealloc();


		memcpy(writePos, FFB(msg)->frames, sizeof(short)*FFB(msg)->numFrames);
		loopLength += FFB(msg)->numFrames;
		writePos += FFB(msg)->numFrames;
	}
}
