#ifndef ___1440958031__
#define ___1440958031__
namespace StrangeIO {
namespace Midi {
/** Basic representation of a MIDI control code */
struct MidiCode {
	char f; ///< function code
	char n; ///< note code
	char v; ///< Velocity of note
};
 
} // Midi
} // StrangeIO
#endif
