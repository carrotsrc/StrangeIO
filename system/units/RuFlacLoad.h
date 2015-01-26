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
		LOADING_CHUNK,

		ERROR,
	};

	WorkState workState;

	SndfileHandle *file;
	sf_count_t bufSize, count;

	char *filename;
	short *buffer, *position, *period;
	int psize;

protected:
	void feedOut();
	void actionLoadFile();
	void actionNextChunk();

public:
	RuFlacLoad();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
};
#endif
