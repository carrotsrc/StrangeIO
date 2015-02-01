#ifndef RUECHO_H
#define RUECHO_H
#include "framework/rack/RackUnit.h"
class RuEcho : public RackoonIO::RackUnit
{
	enum WorkState {
		IDLE,
		READY,
		PRIMING,
		RUNNING,
		BYPASS
	};

	FILE *fp;

	WorkState workState;
	unsigned int mDelay, sampleRate, bufSize, dLevel;
	float feedDecay;
	short *frameBuffer, *fDelay, *processedPeriod, *feedbackPeriod;
	void add(short*, int);
	bool remainder;

	void writeDebugPCM(short);
public:
	RuEcho();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);
};

#endif
