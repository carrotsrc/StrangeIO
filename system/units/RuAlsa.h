#ifndef RUALSA_H
#define RUALSA_H
#include "framework/rack/RackUnit.h"

class RuAlsa : public RackUnit
{
private:
	snd_pcm_t *handle;
	FeedState feedJackAudio();
	std::thread *tAudio;

	void audioFeed();
public:
	RuAlsa();
	FeedState feed(Jack*);
	void setConfig(string,string);
	RackState init();
};
#endif
