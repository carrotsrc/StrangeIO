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
		FLUSH,
		FLUSH_REMAINDER,
		REMAINDER_WAITING
	};
	WorkState workState;
	int nResampled, nFrames, nRemainder, nNormal;
	short *convPeriod, *releasePeriod;
	float *framesIn, *framesOut, *remainder, *remRead;
	double ratio;
	void *resampler;
	void actionResample();

	void overwritePeriod(short *, int, int);

	std::mutex bufLock;

	void midiBend(int);
	inline void sfMemcpy(float*,short*,int);
	inline void fsMemcpy(short*,float*,int);

	FILE *fp;
	FILE *fp2;
	bool dd;
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
