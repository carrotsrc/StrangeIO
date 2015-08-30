#ifndef __ALSADRIVERINTERFACE_HPP_1440957281__
#define __ALSADRIVERINTERFACE_HPP_1440957281__
#include <memory>

#include "framework/fwcommon.hpp"
#include "framework/midi/Midi.hpp"
#include "framework/midi/MidiInputHandle.hpp"
namespace StrangeIO {
namespace Midi {

class DriverUtilityInterface {
public:
	midi_in_uptr open_input_port(std::string dev, std::string name);
	void close_input_port(midi_in_uptr);
};

} // Midi
} // StrangeIO
#endif