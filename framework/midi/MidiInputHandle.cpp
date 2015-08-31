#include "framework/midi/MidiInputHandle.hpp"

using namespace StrangeIO::Midi;
MidiInputHandle::MidiInputHandle(std::string port_name) :
m_port_name(port_name)
{ }

std::string MidiInputHandle::port_name() {
	return m_port_name;
}