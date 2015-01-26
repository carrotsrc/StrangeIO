#ifndef RACK_H
#define RACK_H
#include "RackChain.h"
#include "framework/threads/RackQueue.h"

namespace RackoonIO {
class Rack {
	RackQueue *queue;
public:
	Rack();
	void init();
};
}
#endif 
