#ifndef RUCHANNELMIXER_H
#define RUCHANNELMIXER_H
#include "framework/rack/RackUnit.h"

class RuChannelMixer : public RackoonIO::RackUnit {
	enum WorkState {
		IDLE,
		INIT,
		READY
	};
	short *periodC1, *periodC2, *mixedPeriod;
	float gainC1, gainC2;
	WorkState workState;

public:
	RuChannelMixer();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);
};
#endif
