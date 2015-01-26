#ifndef RUFLACLOAD_H
#define RUFLACLOAD_H
#include "framework/rack/RackUnit.h"
#include <sndfile.hh>
class RuFlacLoad : public RackoonIO::RackUnit
{
	enum WorkState {
		IDLE,
		LOADING,
		READY,
		STREAMING,
	};

	WorkState workState;

	SndfileHandle *file;
	sf_count_t bufSize, count;

	char *filename;
	short *buffer, *position;

	void streamAudio();

protected:
	void feedOut();
	void actionLoadFile();

public:
	RuFlacLoad();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);
	RackoonIO::RackState init();

	void cycle();
};
#endif
