#include "RuFlacLoad.h"

RuFlacLoad::RuFlacLoad()
: RackUnit() {
	addJack("power");
	addPlug("audio_out");
}

FeedState RuFlacLoad::feed(string jack) {

}

void RuFlacLoad::init() {

}
