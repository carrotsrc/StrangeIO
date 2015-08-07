#include "framework/rack/RackUnit.h"
using namespace StrangeIO;
class TestAdaptor : public StrangeIO::RackUnit {

public:
	TestAdaptor();
	RackState init();

	void setConfig(std::string, std::string);

	FeedState feed(Jack *jack);

	RackState cycle();
	void block(Jack*);

protected:
};
