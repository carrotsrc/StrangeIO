#ifndef UNITCONNECTORS_H
#define UNITCONNECTORS_H
#include "common.h"
#include "framework/buffers/TGeneralBuffer.h"
namespace RackoonIO {

class RackUnit;
enum FeedState {
	FEED_OK,
	FEED_WAIT,
	FEED_BLOCKED
};

enum RackState {
	RACK_AC,
	RACK_RESET,
	RACK_OFF,
	RACK_UNIT_OK,
	RACK_UNIT_FAILURE
};

enum ConnectorType {
	JACK_AC,
	JACK_THREADED,
	JACK_SEQ
};

class UnitConnector {
protected:
	RackUnit *weld;
public:
	UnitConnector(RackUnit *wunit)
	{ weld = wunit; }

	string name;
	bool connected;

};

class Jack : public UnitConnector {
public:
	int frames;

	Jack(RackUnit *wunit) : UnitConnector(wunit) {};
	virtual FeedState feed(short*) = 0;
	virtual FeedState flush(short**) = 0;
	RackState rackFeed(RackState state);
};

class SeqJack : public Jack {
protected:
	short *buffer;
public:
	SeqJack(RackUnit *weld) : Jack(weld) {};
	FeedState feed(short *);
	FeedState flush(short **);

};

class ThreadedJack : public Jack {
protected:
	TGeneralBuffer<short*> buffer;
public:
	ThreadedJack(RackUnit *weld) : Jack(weld) {};
	FeedState feed(short *);
	FeedState flush(short **);
};

class AcJack : public Jack {
protected:
	TGeneralBuffer<short*> buffer;
public:
	AcJack(RackUnit *weld) : Jack(weld) {};
	FeedState feed(short *);
	FeedState flush(short **);
};

class Plug : public UnitConnector {
public:
	Plug(RackUnit *wunit) : UnitConnector(wunit) {};
	RackUnit *unit;
	Jack *jack;
};

}
#endif
