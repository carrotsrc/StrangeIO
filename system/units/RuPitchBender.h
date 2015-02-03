#ifndef RUPITCHBENDER_H
#define RUPITCHBENDER_H
#include "framework/rack/RackUnit.h"

class RuPitchBender : public RackoonIO::RackUnit {
	enum WorkState {
		IDLE,
		INIT,
		READY,
		BYPASS
	};
	WorkState workState;
	short *bentPeriod;

	int bend, oldBend, step, inc, nsize;

public:
	RuPitchBender();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);
};
#endif
