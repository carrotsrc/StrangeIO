#ifndef __ALSADRIVERINTERFACE_HPP_1440957281__
#define __ALSADRIVERINTERFACE_HPP_1440957281__
#include <memory>

#include "framework/fwcommon.hpp"
#include "framework/midi/Midi.hpp"
#include "framework/midi/MidiInputHandle.hpp"
namespace StrangeIO {
namespace Midi {

class DriverUtilityInterface {
	midi_in_uptr open_input_port(std::string dev, std::string name);
};

} // Midi
} // StrangeIO
#endif