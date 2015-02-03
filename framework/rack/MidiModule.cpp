#include "MidiModule.h"
#include <iomanip>

using namespace RackoonIO;

MidiModule::MidiModule(string port, string name) {
	portName = port;
	alias = name;
	inMidi = nullptr;
}

bool MidiModule::init() {
	int err = 0;
	if((err = snd_rawmidi_open(&inMidi, NULL, portName.c_str(), SND_RAWMIDI_SYNC|SND_RAWMIDI_NONBLOCK)) < 0) {
		std::cerr << alias << ": Error opening Midi on port " << portName << ": " <<
			snd_strerror(err) << std::endl;
		return false;
	}
	return true;
}

void MidiModule::cycle() {
	MidiCode code = flush();

	if(code.f == 0)
		return;

	try { bindings.at(code.n)(code.v); }
	catch(const std::out_of_range& oor) {}
}


MidiCode MidiModule::flush() {
	int err = 0;
	MidiCode code;
	if((err = snd_rawmidi_read(inMidi, (void*)&code, 3)) < 0) {
		if(err == -EAGAIN || err == -EBUSY)
			return {0,0,0};

		std::cerr << alias << ": Critical error Reading Midi on port " << portName << " - " <<
			snd_strerror(err) << std::endl;
	}

	return code;
}

std::string MidiModule::getAlias() {
	return alias;
}

std::string MidiModule::getPort() {
	return portName;
}

void MidiModule::addBinding(double code, std::function<void(int)> func) {
	bindings.insert(std::pair< int, std::function<void(int)> >((int)code, func));
}
