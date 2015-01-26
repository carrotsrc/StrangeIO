#include "RuFlacLoad.h"
#define CHUNK_SIZE 0x100000
using namespace RackoonIO;

RuFlacLoad::RuFlacLoad()
: RackUnit() {
	addJack("power", JACK_AC);
	addPlug("audio_out");
	workState = IDLE;
	psize = 2048;
}

RackoonIO::FeedState RuFlacLoad::feed(RackoonIO::Jack*jack) {
}

void RuFlacLoad::setConfig(string config, string value) {
	if(config == "filename")
		filename = (char*)value.c_str();
}

void RuFlacLoad::actionNextChunk() {
	period = NULL;
	while(period == NULL) period = (short*)calloc(psize, sizeof(short));

	if(count < psize) psize = count;
	memcpy(period, position, psize<<1);
	count -= psize;
	position += psize;
	workState = STREAMING;
}

void RuFlacLoad::actionLoadFile() {
	file = new SndfileHandle(filename);

	if(file->error() > 0) {
		workState = ERROR;
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
	cout << "RuFlacLoad: Initialised" << endl;
}

RackoonIO::RackState RuFlacLoad::init() {
	workState = LOADING;
	addPackage(std::bind(&RuFlacLoad::actionLoadFile, this));
	return RACK_UNIT_OK;
}

RackoonIO::RackState RuFlacLoad::cycle() {


	if(workState < READY)
		return RACK_UNIT_OK;

	switch(workState) {
	case ERROR:
		return RACK_UNIT_FAILURE;

	case READY:
		workState = LOADING_CHUNK;
		addPackage(std::bind(&RuFlacLoad::actionNextChunk, this));
		break;

	case STREAMING:
		Jack *jack = getPlug("audio_out")->jack;
		if(jack->feed(period) == FEED_OK) {
			workState = LOADING_CHUNK;
			addPackage(std::bind(&RuFlacLoad::actionNextChunk, this));
		}
		break;
	}

	return RACK_UNIT_OK;
}
