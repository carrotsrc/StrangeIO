#ifndef RACKCHAIN_H
#define RACKCHAIN_H
#include "RackUnit.h"

class RackChain
{
	vector<RackUnit*> chain;

public:
	void addUnit(RackUnit*);
	void join();
};
#endif
