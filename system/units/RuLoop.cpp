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
	while(tmp == NULL)
		tmp = (short*) realloc(loopBuffer, loopCapacity + (sampleRate<<1));

	loopBuffer = tmp;
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
	while(workState == LOOPING) {
		if(out->feed(period) == FEED_OK) {
			readPos += bufSize;
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

	if(workState == PRIMING) {
		if( ((loopLength-loopBuffer) + jack->frames) > loopCapacity)
			bufferRealloc();
		memcpy(writePos, period, sizeof(short)*jack->frames);
		loopLength += jack->frames;
		cout << "Stored " << jack->frames << " frames" << endl;
	}
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
		cout << "RuLoop: PRIMING" << endl;
	}
	else
	if(workState == PRIMING) {
		workState = READY;
		cout << "RuLoop: READY" << endl;
	}
}

void RuLoop::midiToggleLoop(int value) {
	if(value < 127)
		return;

	if(workState == READY) {
		workState = LOOPING;
		cout << "RuLoop: LOOPING" << endl;
	}
	else if(workState == LOOPING) {
		workState = READY;
		cout << "RuLoop: READY" << endl;
	} else {
		cout << "Bad state change" << endl;
	}
}

void RuLoop::eventFinalBuffer(std::shared_ptr<EventMessage> msg) {
	CONSOLE_MSG("RuLoop", "Received transfer - " << (((EvFramesFinalBuffer*)msg.get())->numFrames) << " frames");
}
