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
public:
	MidiModule(string, string);
	void init();
	MidiCode cycle();

	std::string getPort();
	std::string getAlias();
};

}
#endif
