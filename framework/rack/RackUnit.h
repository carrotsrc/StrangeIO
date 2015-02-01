#ifndef RACKUNIT_H
#define RACKUNIT_H
#include <functional>

#include "framework/buffers/TGeneralBuffer.h"
#include "UnitConnectors.h"
#include "framework/threads/RackQueue.h"

namespace RackoonIO {
#define MIDI_BIND(func) (new std::function<void(char)>(std::bind(&func, this)))
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
	std::map<string, std::function<void(int)> > midiExport;

protected:
	RackChain *chain;
	UnitState unitState;

	void addJack(string, ConnectorType);
	void addPlug(string);

	void feedOut(string, short*);

	void outsource(std::function<void()>);
	void midiExportMethod(string, std::function<void(int)> );

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

	bool midiControllable();
	virtual std::map<string, std::function<void(int)> > midiExportedMethods();

	virtual void setConfig(string, string) = 0;
	virtual FeedState feed(Jack*) = 0;

	virtual RackState cycle() = 0;
	virtual RackState init() = 0;

	virtual void block(Jack*) = 0;
};

}
#endif
