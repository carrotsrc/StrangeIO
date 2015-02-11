#ifndef EVENTMESSAGE_H
#define EVENTMESSAGE_H

namespace RackoonIO {

typedef short EventType;

class EventMessage {
public:
	virtual ~EventMessage() { msgType = 0; };
	EventType msgType;
};

}
#endif
