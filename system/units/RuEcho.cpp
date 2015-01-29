#include "RuEcho.h"
using namespace RackoonIO;

RuEcho::RuEcho()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
}

FeedState RuEcho::feed(RackoonIO::Jack *jack) {
	if(workState == BYPASS) {
		short *tbuf;
		if(jack->flush(&tbuf) == FEED_OK) {
			Jack *out = getPlug("audio_out")->jack;
			out->frames = jack->frames;
			return out->feed(tbuf);
		}
	}
}

void RuEcho::setConfig(string,string) {

}

RackState RuEcho::init() {
	workState = BYPASS;
	cout << "RuEcho: Initialised" << endl;
}

RackState RuEcho::cycle() {

}
