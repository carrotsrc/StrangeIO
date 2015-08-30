#ifndef ___1440958031__
#define ___1440958031__

/** Basic representation of a MIDI control code */
struct MidiCode {
	char f; ///< function code
	char n; ///< note code
	char v; ///< Velocity of note
};
 
#endif
