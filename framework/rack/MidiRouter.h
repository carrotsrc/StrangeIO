#ifndef MIDIROUTER_H
#define MIDIROUTER_H
#include "MidiModule.h"

namespace RackoonIO {

class MidiRouter {
	std::vector<MidiModule*> modules;

public:
	void addModule(std::string, std::string);
	void addBinding(std::string, double, std::function<void(int)>);
	MidiModule* operator[] (std::string);

	void init();
	void cycle();
};

}
#endif
