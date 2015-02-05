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
	int sampleRate, convRate;
	short *convPeriod;
	float *framesIn, *framesOut;
	double ratio;
public:
	RuPitchBender();
	~RuPitchBender();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);
};
#endif
