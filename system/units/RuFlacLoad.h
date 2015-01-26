#ifndef RUFLACLOAD_H
#define RUFLACLOAD_H
#include "framework/rack/RackUnit.h"
#include <sndfile.hh>
class RuFlacLoad : public RackoonIO::RackUnit
{
	SndfileHandle *file;
	sf_count_t bufSize, count;

	char *filename;
	short *buffer, *position;
	std::thread *tLoader;

	void streamAudio();

protected:
	void feedOut();

public:
	RuFlacLoad();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);
	RackoonIO::RackState init();

	void cycle() {

	}
};
#endif
