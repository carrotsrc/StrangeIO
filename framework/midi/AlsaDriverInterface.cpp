#include "framework/midi/AlsaDriverInterface.hpp"
#include "framework/midi/AlsaInputHandle.hpp"

using namespace StrangeIO::Midi;
#include <iostream>
midi_in_uptr DriverUtilityInterface::open_input_port(std::string dev, std::string name) {
	snd_rawmidi_t* handle = nullptr;
	auto status = 0;
	
	if((status = snd_rawmidi_open(&handle, nullptr, name.c_str(), SND_RAWMIDI_SYNC)) < 0) {
		std::cerr << dev << ": Error opening Midi on port " << name << ": " <<
		snd_strerror(status) << std::endl;
		return midi_in_uptr(nullptr);
	}
	snd_rawmidi_drain(handle);
	return midi_in_uptr(new AlsaInputHandle(handle, name));
}

void DriverUtilityInterface::close_input_port(midi_in_uptr in) {
	auto handle = static_cast<AlsaInputHandle*>(in.get())->handle();
	snd_rawmidi_close(handle);
}