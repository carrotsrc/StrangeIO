#ifndef RUECHO_H
#define RUECHO_H
#include "framework/rack/RackUnit.h"
class RuEcho : public RackoonIO::RackUnit
{
	enum WorkState {
		IDLE,
		READY,
		RUNNING,
		BYPASS
	};

	WorkState workState;

public:
	RuEcho();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
};

#endif
