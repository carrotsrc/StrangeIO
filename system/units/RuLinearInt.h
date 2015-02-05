#ifndef RULINEARINT_H
#define RULINEARINT_H
#include "framework/rack/RackUnit.h"

class RuLinearInt : public RackoonIO::RackUnit {
	enum WorkState {
		IDLE,
		INIT,
		READY,
		BYPASS
	};
	WorkState workState;

public:
	RuLinearInt();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);
};
#endif
