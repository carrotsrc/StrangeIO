#ifndef MIDIROUTER_H
#define MIDIROUTER_H
#include "MidiModule.h"

namespace RackoonIO {

class MidiRouter {
	std::vector<MidiModule> modules;

public:
	void addModule(string, string);
}

}
#endif
