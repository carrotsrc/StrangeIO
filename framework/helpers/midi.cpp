#include "midi.h"
using namespace strangeio::Helpers;
float MidiRoutines::normaliseVelocity64(int value) {
	if(value == 64) {
		return 0.0f;
	} else {
		return ( (value < 64.0f) ?
			-((64.0f-value)*(0.015625f)) :
			((value-63.0f)*(0.015625)));
	}

}

float MidiRoutines::normaliseVelocity128(int value) {
	if(value == 64) {
		return 0.5f;
	} else {
		return value/127.0f;
	}

}
