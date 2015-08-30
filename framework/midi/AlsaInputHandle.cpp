#include "framework/midi/AlsaInputHandle.hpp"

using namespace StrangeIO::Midi;

AlsaInputHandle::AlsaInputHandle(snd_rawmidi_t* handle, std::string port_name) :
MidiInputHandle(port_name), m_handle(handle) { }

MidiCode AlsaInputHandle::flush() {
	return MidiCode{0};
}