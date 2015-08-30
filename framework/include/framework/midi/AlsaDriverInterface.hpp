#ifndef __ALSADRIVERINTERFACE_HPP_1440957281__
#define __ALSADRIVERINTERFACE_HPP_1440957281__
#include "framework/fwcommon.hpp"
#include "framework/midi/Midi.hpp"
namespace StrangeIO {
namespace Midi {

class DriverUtilityInterface {
	int open_input(std::string name);
	MidiCode flush_input();
};

} // Midi
} // StrangeIO
#endif