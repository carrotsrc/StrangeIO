#ifndef RUFLACLOAD_H
#define RUFLACLOAD_H
#include "framework/rack/RackUnit.h"
class RuFlacLoad : public RackUnit
{
protected:
	void feedOut();
public:
	RuFlacLoad();
	FeedState feed(string);
	void init();
};
#endif
