#ifndef RACKUNIT_H
#define RACKUNIT_H
#include "framework/threads/TGeneralBuffer.h"
#include "UnitConnectors.h"

class RackChain;
enum UnitState {
	UNIT_ACTIVE,
	UNIT_OFF
};

class RackUnit
{
	vector<Jack*> jackArray;
	vector<Plug*> plugArray;
	string name;

protected:
	RackChain *chain;
	UnitState unitState;

	void addJack(string, ConnectorType);
	void addPlug(string);

	void feedOut(string, short*);



public:
	RackUnit();
	RackUnit(string);

	void setName(string);
	string getName() const;

	void setChain(RackChain*);
	RackChain *getChain() const;

	void printJacks();

	Jack *getJack (string) const;
	Plug *getPlug (string) const;

	void setConnection(string, string, RackUnit*);

	void join();
	void unjoin();

	RackState rackFeed(RackState);

	virtual void setConfig(string, string) = 0;
	virtual FeedState feed(Jack*) = 0;
	virtual RackState init() = 0;
};
#endif
