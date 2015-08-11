#ifndef __OUTPUTUNIT_H_1439299905__
#define __OUTPUTUNIT_H_1439299905__
#include "framework/rack/RackUnit.h"
#include "TestingUnit.h"
 
class OutputUnit : public StrangeIO::Testing::TestingUnit
{
public:
	enum WorkState {
		IDLE,
		INIT,
		READY
	};

	OutputUnit();
	StrangeIO::FeedState feed(StrangeIO::Jack*);
	void setConfig(std::string, std::string);
	StrangeIO::RackState init();
	StrangeIO::RackState cycle();
	void block(StrangeIO::Jack*);
private:
	WorkState workState;
};
#endif

