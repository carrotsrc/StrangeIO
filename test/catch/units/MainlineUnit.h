#ifndef __MAINLINEUNIT_H_1439299831__
#define __MAINLINEUNIT_H_1439299831__
#include "framework/rack/RackUnit.h"
class MainlineUnit : public StrangeIO::RackUnit
{
public:
	enum WorkState {
		IDLE,
		INIT,
		READY
	};

	MainlineUnit();
	StrangeIO::FeedState feed(StrangeIO::Jack*);
	void setConfig(std::string, std::string);
	StrangeIO::RackState init();
	StrangeIO::RackState cycle();
	void block(StrangeIO::Jack*);
private:
	WorkState workState;

	void exportedMethod(int value);
};
#endif
