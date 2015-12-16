#include "framework/midi/alsa_output.hpp"
#include <iostream>
using namespace strangeio::midi;

alsa_output::alsa_output(snd_rawmidi_t* handle, std::string port_name) :
output_handle(port_name), m_handle(handle) { }

void alsa_output::write(msg chr) {

	auto status = snd_rawmidi_write(m_handle, (void*)&chr, 3);
	if( status < 0 ) {
			if( status == -EAGAIN  || status == -EBUSY )
			return;

			std::cerr << port_name() << ": Critical error Writing to Midi on port - " <<
			snd_strerror(status) << std::endl;
			return;
	}
	
	snd_rawmidi_drain(m_handle);
	
	return;
}

snd_rawmidi_t* alsa_output::handle() {
	return m_handle;
}