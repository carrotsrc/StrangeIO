#ifndef RACKCHAIN_H
#define RACKCHAIN_H
#include "RackUnit.h"
namespace RackoonIO {

class RackChain
{
	vector<RackUnit*> chain;

	int sampleRate;
public:
	void addUnit(RackUnit*);
	void join();
};
}
#endif
