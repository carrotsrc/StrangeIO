#ifndef UNITCONNECTORS_H
#define UNITCONNECTORS_H
class RackUnit;
enum FeedState {
	CT_OK,
	CT_WAIT,
	CT_BLOCKED
};

class UnitConnector {
protected:
	RackUnit *weld;
public:
	UnitConnector(RackUnit *wunit) {
		weld = weld;
	}

	string name;
	bool connected;
};

class Jack : public UnitConnector {
public:
	Jack(RackUnit *wunit) : UnitConnector(wunit) {};
	int frames;
	TGeneralBuffer<short*> buffer;
};

class Plug : public UnitConnector {
public:
	Plug(RackUnit *wunit) : UnitConnector(wunit) {};
	RackUnit *unit;
	Jack *jack;
};
#endif
