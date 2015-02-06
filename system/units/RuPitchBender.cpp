#include "RuPitchBender.h"

using namespace RackoonIO;

RuPitchBender::RuPitchBender() : RackUnit() { addJack("audio", JACK_SEQ);
	addPlug("audio_out"); workState = IDLE; framesIn = framesOut = nullptr;
	sampleRate = 44100; convRate = 44100; ratio =
		(double)convRate/(double)sampleRate; convPeriod = nullptr;
	resampler = nullptr; MIDI_BIND("pitchBend", RuPitchBender::midiBend); }

RuPitchBender::~RuPitchBender() {

	if(framesIn != nullptr) { free(framesIn); free(framesOut); } }

void RuPitchBender::actionResample() {
	bufLock.lock();
	int usedFrames = 0; 
	cout << "Resampling...";
	nResampled = resample_process(resampler, ratio, framesIn, nFrames, 0, &usedFrames,
			framesOut, nFrames<<1);
	cout << " Done" << endl;
	convPeriod = (short*)malloc(sizeof(short)*nFrames);

	if(nExcess > 0) {
		/* here we put whatever was left over last round
		 * into the converted buffer
		 */
		cout << "Dumping Excess " << nExcess << " frames... ";
		int i;
		for(i = 0; i < nExcess && i < nFrames; i++) {
			convPeriod[i] = framesXs[i];
		}

		cout << "Dumped " << i << " frames" << endl;

		
		/* set the space left */
		nFrames -= nExcess;
		cout << "Residial: " << nFrames << endl;
		if(nFrames < 0)
			nFrames = 0; // the converted buffer is already full
	}

	cout << "This round: " << nResampled << endl;
	if(nResampled >= nFrames) {
		int i;
		cout << "Greater/equal" << endl;
		for(i = nExcess; i < nFrames; i++)
			convPeriod[i] = framesOut[i];

		nExcess = 0;
		for(i = nFrames; i < nResampled; i++)
			framesXs[nExcess++] = framesOut[i];
		cout << "\tStored " << i << " frames" << endl; 

		workState = FLUSHING;

	} else {
		cout << "Less than" << endl;
		nExcess = 0;
		for(int i = 0; i < nResampled; i++)
			framesXs[nExcess++] = framesOut[i];
		cout << "\tExcess " << nExcess << endl; 
		cout << "Waiting" << endl;
		workState = WAITING;
	}

	bufLock.unlock();

}


FeedState RuPitchBender::feed(Jack *jack) {
	if(!bufLock.try_lock())
		return FEED_WAIT;

	if(workState != READY && workState != WAITING) {
		bufLock.unlock();
		return FEED_WAIT;
	}

	nFrames = jack->frames;
	short *period;

	jack->flush(&period);
	if(ratio == 1) {
		Jack *out = getPlug("audio_out")->jack;
		out->frames = jack->frames;
		bufLock.unlock();
		return out->feed(period);
	}

	
	if(framesOut == nullptr) {
		framesOut = (float*)malloc(sizeof(float)*(nFrames<<1));
		framesIn = (float*)malloc(sizeof(float)*(nFrames));
		framesXs = (float*)malloc(sizeof(float)*(nFrames)<<4);
		nNormal = jack->frames;
	}

	for(int i = 0; i < nFrames; i++)
		framesIn[i] = period[i];

	free(period);
	bufLock.unlock();

	OUTSRC(RuPitchBender::actionResample);
	workState = RESAMPLING;

	return FEED_OK;
}

RackState RuPitchBender::init() {
	resampler = resample_open(1, 0.92, 1.08);
	int fwidth = resample_get_filter_width(resampler);
	cout << "RuPitchBender: Initialised" << endl;
	workState = READY;
	return RACK_UNIT_OK;
}

RackState RuPitchBender::cycle() {
	if(workState == FLUSHING) {
		Jack *out = getPlug("audio_out")->jack;
		out->frames = nNormal;
		if(out->feed(convPeriod) == FEED_OK) {
			cout << "Flushing" << endl;
			workState = READY;
		}
	}

	return RACK_UNIT_OK;
}

void RuPitchBender::setConfig(string config, string value) {

}

void RuPitchBender::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}

void RuPitchBender::midiBend(int value) {
	if(value == 64) {
		ratio = 1;
	}

	if(value < 64) {
		ratio = 1-((64-(double)value)*.00125);
	} else {
		ratio = 1+((64-(127-(double)value))*.00125);
	}
}
