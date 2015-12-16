#ifndef __MIDIINPUTHANDLE_HPP_1440957790__
#define __MIDIINPUTHANDLE_HPP_1440957790__

#include "framework/fwcommon.hpp"
#include "framework/midi/midi.hpp"
#include <memory>

namespace strangeio {
namespace midi {

class output_handle {
public:
	output_handle(std::string port_name);
	virtual msg flush() = 0;

	std::string port_name();

private:
	std::string m_port_name;
	
};

using midi_out_uptr = std::unique_ptr<output_handle>;

} // Midi
} // StrangeIO

#endif

