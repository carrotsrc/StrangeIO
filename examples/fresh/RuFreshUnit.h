#ifndef RUFRESH_H
#define RUFRESH_H
#include "framework/rack/RackUnit.h"

class RuFresh : public RackoonIO::RackUnit
{
public:
	enum WorkState {
		IDLE,
		INIT,
		READY
	};

	RuFresh();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);

private:
	WorkState workState;
};

#endif 
