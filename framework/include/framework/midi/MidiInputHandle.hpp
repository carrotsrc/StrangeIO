#ifndef __MIDIINPUTHANDLE_HPP_1440957790__
#define __MIDIINPUTHANDLE_HPP_1440957790__

#include "framework/fwcommon.hpp"
#include "framework/midi/Midi.hpp"
#include <memory>

namespace StrangeIO {
namespace Midi {

class MidiInputHandle {
public:
	MidiInputHandle(std::string port_name);
	virtual MidiCode flush() = 0;

	std::string port_name();

private:
	std::string m_port_name;
	
};

using midi_in_uptr = std::unique_ptr<MidiInputHandle>;

} // Midi
} // StrangeIO

#endif

