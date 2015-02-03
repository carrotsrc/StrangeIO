#ifndef MIDIMODULE_H
#define MIDIMODULE_H
#include "common.h"
namespace RackoonIO {

typedef struct {
	char f;
	char n;
	char v;
} MidiCode;

class MidiModule {
	snd_rawmidi_t *inMidi;
	std::string portName;
	std::string alias;
	char buffer[3];

	std::map<int, std::function<void(int)> > bindings;
public:
	MidiModule(string, string);
	bool init();
	void cycle();
	MidiCode flush();

	std::string getPort();
	std::string getAlias();
	void addBinding(double, std::function<void(int)>);
};

}
#endif
