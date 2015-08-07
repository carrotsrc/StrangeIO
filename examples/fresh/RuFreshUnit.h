#ifndef RUFRESH_H
#define RUFRESH_H
#include "framework/rack/RackUnit.h"

class RuFresh : public StrangeIO::RackUnit
{
public:
	enum WorkState {
		IDLE,
		INIT,
		READY
	};

	RuFresh();
	StrangeIO::FeedState feed(StrangeIO::Jack*);
	void setConfig(string,string);

	StrangeIO::RackState init();
	StrangeIO::RackState cycle();
	void block(StrangeIO::Jack*);

private:
	WorkState workState;
};

#endif
