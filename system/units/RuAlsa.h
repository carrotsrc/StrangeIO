#ifndef RUALSA_H
#define RUALSA_H
#include "framework/rack/RackUnit.h"

class RuAlsa : public RackoonIO::RackUnit
{
	enum WorkState {
		IDLE,
		INIT,
		READY,
		PRIMING,
		STREAMING,
		FLUSHING
	};

	WorkState workState;
	snd_pcm_t *handle;
	unsigned int sampleRate, mLatency, bufSize, bufLevel, maxPeriod;
	short *frameBuffer;
	snd_pcm_uframes_t triggerLevel, fPeriod;

	//snd_async_handler_t *pcm_callback;

	RackoonIO::FeedState feedJackAudio();
	std::mutex bufLock;

	void audioFeed();

	void actionInitAlsa();
	void actionFlushBuffer();

//	void asyncCallback();
public:
	RuAlsa();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
};

//void RuAlsaCallback(snd_async_handler_t *pcm_callback);
#endif
