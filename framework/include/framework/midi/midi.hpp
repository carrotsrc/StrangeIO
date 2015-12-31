#ifndef ___1440958031__
#define ___1440958031__
#include "framework/fwcommon.hpp"
namespace strangeio {
namespace midi {
/** Basic representation of a MIDI control code */
struct msg {
	uint8_t f; ///< function code
	uint8_t n; ///< note code
	uint8_t v; ///< Velocity of note
};
 
} // Midi
} // StrangeIO
#endif
