#ifndef ___1440958031__
#define ___1440958031__
namespace strangeio {
namespace midi {
/** Basic representation of a MIDI control code */
struct msg {
	char f; ///< function code
	char n; ///< note code
	char v; ///< Velocity of note
};
 
} // Midi
} // StrangeIO
#endif
