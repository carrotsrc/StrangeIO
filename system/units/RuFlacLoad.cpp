#include "RuFlacLoad.h"
#define CHUNK_SIZE 0x100000
using namespace RackoonIO;

RuFlacLoad::RuFlacLoad()
: RackUnit() {
	addJack("power", JACK_AC);
	addPlug("audio_out");
	workState = IDLE;
}

RackoonIO::FeedState RuFlacLoad::feed(RackoonIO::Jack*jack) {
}

void RuFlacLoad::setConfig(string config, string value) {
	if(config == "filename")
		filename = (char*)value.c_str();
}

void RuFlacLoad::streamAudio() {
	cout << "Audio stream running" << endl;
	Jack *jack = getPlug("audio_out")->jack;
	short *period = NULL;
	signed int psize = 2048;
	count = bufSize;

	period = NULL;
	while(period == NULL) period = (short*)calloc(psize, sizeof(short));
	if(count < psize) psize = count;
	memcpy(period, position, psize<<1);

	while(unitState == UNIT_ACTIVE) {
		if(jack->feed(period) == FEED_OK) {
			period = NULL;
			while(period == NULL) period = (short*)calloc(psize, sizeof(short));

			if(count < psize) psize = count;
			memcpy(period, position, psize<<1);
			count -= psize;
			position += psize;
		}

		if(psize <= 0)
			break;
	}
}

void RuFlacLoad::actionLoadFile() {
	cout << "Loading `" << filename << "`" << endl;
	file = new SndfileHandle(filename);

	if(file->error() > 0) {
		cout << "Failed to load file" << endl;
		return;
	}


	bufSize = file->frames()<<1;

	buffer = (short*)calloc(bufSize, sizeof(short));
	position = buffer;

	while(file->read(position, CHUNK_SIZE) == CHUNK_SIZE) {
		position += CHUNK_SIZE;
	}
	position = buffer;
	workState = READY;
}

RackoonIO::RackState RuFlacLoad::init() {
	cout << "Initialising RuFlacLoad "<<endl;
	workState = LOADING;
	addPackage(std::bind(&RuFlacLoad::actionLoadFile, this));
}

void RuFlacLoad::cycle() {
	if(workState == READY) {
		cout << "Ready" << endl;
		workState = STREAMING;
	}
}
