#ifndef __MIDIINPUTHANDLE_HPP_1440957790__
#define __MIDIINPUTHANDLE_HPP_1440957790__

#include "framework/fwcommon.hpp"
#include "framework/midi/midi.hpp"
#include <memory>

namespace strangeio {
namespace midi {

class input_handle {
public:
	input_handle(std::string port_name);
	virtual msg flush() = 0;

	std::string port_name();

private:
	std::string m_port_name;
	
};

using midi_in_uptr = std::unique_ptr<input_handle>;

} // Midi
} // StrangeIO

#endif

