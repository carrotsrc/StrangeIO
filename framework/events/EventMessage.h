#ifndef EVENTMESSAGE_H
#define EVENTMESSAGE_H

namespace RackoonIO {

typedef short EventType;

typedef struct {
	EventType msgType;
} EventMessage;

}
#endif
