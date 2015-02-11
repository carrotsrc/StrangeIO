#ifndef RULOOP_H
#define RULOOP_H
#include "framework/rack/RackUnit.h"

class RuLoop : RackoonIO::RackUnit {
	enum WorkState {
		IDLE,
		INIT,
		READY,
		PRIMING,
		LOOPING,
		PASSTHROUGH
	};

	WorkState workState;
	short *loopBuffer, *writePos, *readPos, *loopLength;
	int sampleRate, loopCapacity, frames;

	void bufferRealloc();
	void feedLoop();

	void eventFinalBuffer(std::shared_ptr<RackoonIO::EventMessage>);
public:
	RuLoop();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);

	void midiToggleRecord(int);
	void midiToggleLoop(int);

};
#endif
