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
	string portName;
	char buffer[3];
public:
	MidiModule(string);
	void init();
	void cycle();
};

}
#endif
