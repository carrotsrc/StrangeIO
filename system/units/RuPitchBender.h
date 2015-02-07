#ifndef RUPITCHBENDER_H
#define RUPITCHBENDER_H
#include "framework/rack/RackUnit.h"
#include <libresample.h>
class RuPitchBender : public RackoonIO::RackUnit {
	enum WorkState {
		IDLE,
		INIT,
		READY,
		WAITING,
		RESAMPLING,
		FLUSHING,
		RELEASE
	};
	WorkState workState;
	int nResampled, nFrames, nExcess, nNormal;
	short *convPeriod, *releasePeriod;
	float *framesIn, *framesOut, *framesXs;
	double ratio;
	void *resampler;
	void actionResample();

	std::mutex bufLock;

	void midiBend(int);
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
