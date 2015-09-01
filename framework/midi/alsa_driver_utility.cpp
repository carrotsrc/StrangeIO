#include "framework/midi/alsa_driver_utility.hpp"
#include "framework/midi/alsa_input.hpp"

using namespace strangeio::midi;
#include <iostream>

driver_utility::driver_utility() {

}
midi_in_uptr driver_utility::open_input_port(std::string dev, std::string name) {
	snd_rawmidi_t* handle = nullptr;
	auto status = 0;
	
	if((status = snd_rawmidi_open(&handle, nullptr, name.c_str(), SND_RAWMIDI_SYNC)) < 0) {
		std::cerr << dev << ": Error opening Midi on port " << name << ": " <<
		snd_strerror(status) << std::endl;
		return midi_in_uptr(nullptr);
	}

	snd_rawmidi_drain(handle);
	return midi_in_uptr(new alsa_input(handle, name));
}

void driver_utility::close_input_port(midi_in_uptr in) {
	auto handle = static_cast<alsa_input*>(in.get())->handle();
	snd_rawmidi_close(handle);
}