#ifndef __MIDI_H_1438891287__
#define __MIDI_H_1438891287__

#include "framework/fwcommon.hpp"
namespace strangeio {
namespace routine {
namespace midi {
	float normalise_velocity64(int value);
	float normalise_velocity128(int value);
}
} // helpers
} // StrangeIO
#endif
