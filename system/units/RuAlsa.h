#ifndef RUALSA_H
#define RUALSA_H
#include "framework/rack/RackUnit.h"

class RuAlsa : public RackoonIO::RackUnit
{
	enum WorkState {
		IDLE,
		INIT,
		READY,
		STREAMING
	};

	WorkState workState;
	snd_pcm_t *handle;
	unsigned int sampleRate, mLatency, bSize,
				bLevel, bExcess;

	short *bufA, *bufB, *bufExcess, *bufPosition;
	short cBuffer;

	RackoonIO::FeedState feedJackAudio();
	std::mutex bufLock;

	void audioFeed();

	void actionInitAlsa();
	void actionFlushBuffer();

	inline void handleBuffers(RackoonIO::Jack*, short*);
	void bufferSwitch();
public:
	RuAlsa();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
};
#endif
