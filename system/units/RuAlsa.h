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
	RackoonIO::FeedState feedJackAudio();
	std::thread *tAudio;

	void audioFeed();

	void actionInitAlsa();
public:
	RuAlsa();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);
	RackoonIO::RackState init();
	void cycle();
};
#endif
