#ifndef RULEVELS_H
#define RULEVELS_H
class RuLevels : public RackoonIO::RackUnit
{
	enum WorkState {
		IDLE,
		READY
	};

	WorkState workState;
	snd_pcm_t *handle;

public:
	RuLevels();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
};
#endif
