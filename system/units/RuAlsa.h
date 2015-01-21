#ifndef RUALSA_H
#define RUALSA_H
#include "framework/rack/RackUnit.h"

class RuAlsa : public RackUnit
{
private:
	snd_pcm_t *handle;

	FeedState feedJackAudio();
public:
	RuAlsa();
	FeedState feed(string);
	void init();
};
#endif
