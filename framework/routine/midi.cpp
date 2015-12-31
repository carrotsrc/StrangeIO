#include "framework/routine/midi.hpp"
using namespace strangeio::routine;
float midi::normalise_velocity64(int value) {
	if(value == 64) {
		return 0.0f;
	} else {
		return ( (value < 64.0f) ?
			-((64.0f-value)*(0.015625f)) :
			((value-63.0f)*(0.015625)));
	}

}

float midi::normalise_velocity128(int value) {
	if(value == 64) {
		return 0.5f;
	} else {
		return value/127.0f;
	}

}
