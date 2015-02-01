#include "MidiModule.h"
#include <iomanip>

using namespace RackoonIO;

MidiModule::MidiModule(string name) {
	portName = name;
	inMidi = nullptr;
}

void MidiModule::init() {

	int err = 0;
	if((err = snd_rawmidi_open(&inMidi, NULL, portName.c_str(), SND_RAWMIDI_SYNC|SND_RAWMIDI_NONBLOCK)) < 0) {
		std::cerr << "Error opening Midi on port " << portName << ": " <<
			snd_strerror(err) << std::endl;
	}
}

MidiCode MidiModule::cycle() {
	int err = 0;
	MidiCode code;
	if((err = snd_rawmidi_read(inMidi, (void*)&code, 3)) < 0) {
		if(err == -EAGAIN || err == -EBUSY)
			return;

		std::cerr << "Critical error Reading Midi on port " << portName << ": " <<
			snd_strerror(err) << std::endl;
	}

	return code;
}
