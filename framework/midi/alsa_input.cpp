#include "framework/midi/alsa_input.hpp"
#include <iostream>
using namespace strangeio::midi;

alsa_input::alsa_input(snd_rawmidi_t* handle, std::string port_name) :
input_handle(port_name), m_handle(handle) { }

msg alsa_input::flush() {
	msg code;
	auto status = snd_rawmidi_read(m_handle, (void*)&code, 3);
	if( status < 0 ) {
			if( status == -EAGAIN  || status == -EBUSY )
			return msg{0};

			std::cerr << port_name() << ": Critical error Reading Midi on port - " <<
			snd_strerror(status) << std::endl;
			return msg{0};
	}
	
	return code;
}

snd_rawmidi_t* alsa_input::handle() {
	return m_handle;
}