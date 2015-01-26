#ifndef RACKUNIT_H
#define RACKUNIT_H
#include <functional>

#include "framework/buffers/TGeneralBuffer.h"
#include "UnitConnectors.h"
#include "framework/threads/RackQueue.h"

namespace RackoonIO {

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

	RackQueue *rackQueue;

protected:
	RackChain *chain;
	UnitState unitState;

	void addJack(string, ConnectorType);
	void addPlug(string);

	void feedOut(string, short*);

	void addPackage(std::function<void()>);


public:
	RackUnit();
	RackUnit(string);

	void setName(string);
	string getName() const;

	void setChain(RackChain*);
	RackChain *getChain() const;

	void setRackQueue(RackQueue*);

	Jack *getJack (string) const;
	Plug *getPlug (string) const;

	void setConnection(string, string, RackUnit*);

	void join();
	void unjoin();

	RackState rackFeed(RackState);

	virtual void setConfig(string, string) = 0;
	virtual FeedState feed(Jack*) = 0;

	virtual RackState cycle() = 0;
	virtual RackState init() = 0;
};

}
#endif
