#ifndef __ALSADRIVERINTERFACE_HPP_1440957281__
#define __ALSADRIVERINTERFACE_HPP_1440957281__
#include <memory>

#include "framework/fwcommon.hpp"
#include "framework/midi/Midi.hpp"
#include "framework/midi/MidiInputHandle.hpp"
namespace StrangeIO {
namespace Midi {

class DriverUtilityInterface {
	std::unique_ptr<MidiInputHandle> open_input_port(std::string name);
};

} // Midi
} // StrangeIO
#endif