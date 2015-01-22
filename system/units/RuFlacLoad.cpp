#include "RuFlacLoad.h"
#define CHUNK_SIZE 0x100000
RuFlacLoad::RuFlacLoad()
: RackUnit() {
	addJack("power", JACK_AC);
	addPlug("audio_out");
}

FeedState RuFlacLoad::feed(Jack*jack) {
}

void RuFlacLoad::setConfig(string config, string value) {
	if(config == "filename")
		filename = (char*)value.c_str();
}

RackState RuFlacLoad::init() {
	cout << "Initialising RuFlacLoad" << endl;
	file = new SndfileHandle(filename);

	bufSize = file->frames()<<1;

	buffer = (short*)calloc(bufSize, sizeof(short));
	position = buffer;

	while(file->read(position, CHUNK_SIZE) == CHUNK_SIZE) {
		position += CHUNK_SIZE;
	}

	position = buffer;
	free(buffer);
}
