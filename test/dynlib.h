#include "framework/rack/RackUnit.h"
using namespace RackoonIO;
class TestAdaptor : public RackoonIO::RackUnit {

public:
	TestAdaptor();
	RackState init();

	void setConfig(std::string, std::string);

	FeedState feed(Jack *jack);

	RackState cycle();
	void block(Jack*);

protected:
};
