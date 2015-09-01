#include "framework/midi/AlsaInputHandle.hpp"
#include <iostream>
using namespace strangeio::Midi;

AlsaInputHandle::AlsaInputHandle(snd_rawmidi_t* handle, std::string port_name) :
MidiInputHandle(port_name), m_handle(handle) { }

MidiCode AlsaInputHandle::flush() {
	MidiCode code;
	if(auto status = snd_rawmidi_read(m_handle, (void*)&code, 3) < 0) {
			if(status == -EAGAIN || status == -EBUSY)
			return MidiCode{0};

			std::cerr << port_name() << ": Critical error Reading Midi on port - " <<
			snd_strerror(status) << std::endl;
			return MidiCode{0};
	}
	
	return code;
}

snd_rawmidi_t* AlsaInputHandle::handle() {
	return m_handle;
}