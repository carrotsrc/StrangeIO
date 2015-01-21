#ifndef RACKUNIT_H
#define RACKUNIT_H
#include "framework/threads/TGeneralBuffer.h"
#include "UnitConnectors.h"

class RackChain;
class RackUnit
{
	vector<Jack> jackArray;
	vector<Plug> plugArray;
	string name;

protected:
	RackChain *chain;

	void addJack(string);
	void addPlug(string);

	void feedOut(string, short*);

public:
	RackUnit();
	RackUnit(string);

	void setName(string);
	string getName();

	void setChain(RackChain*);
	RackChain *getChain();

	void printJacks();

	Jack *getJack(string);
	Plug *getPlug(string);

	void setConnection(string, string, RackUnit*);

	void join();
	void unjoin();

	virtual FeedState feed(string) = 0;
	virtual void init() = 0;
};
#endif
