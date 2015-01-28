#include "RuAlsa.h"
using namespace RackoonIO;

RuAlsa::RuAlsa()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	workState = IDLE;
	sampleRate = 44100;
	mLatency = bSize = bLevel = bExcess = misses = 0;
	bufA = bufB = bufPosition = NULL;
	cBuffer = 0;
}

RackoonIO::FeedState RuAlsa::feed(RackoonIO::Jack *jack) {
	Jack *j = getJack("audio");
	short *period;
	int bytes;
	if(j->flush(&period) == FEED_OK) {
		if(!bSize) {
			if((bytes = snd_pcm_mmap_writei(handle, period, (0x100<<1))) < (0x100<<1)) {
				if(bytes == -EPIPE)
					cerr <<" Underrun occurred" << endl;
			}
		} else 
			handleBuffers(j, period);

		free(period);
		period = NULL;
	}

	return FEED_OK;
}

inline void RuAlsa::handleBuffers(Jack *j, short *period) {
	int nFrames = j->frames;

	if(bExcess) {
		memcpy(bufPosition, bufExcess, bExcess);
		bLevel += bExcess;
		bufPosition += bExcess;
		bExcess = 0;
	}

	if((bLevel + j->frames) > bSize) {
		nFrames = bSize - bLevel;
		bExcess = j->frames - nFrames;
		memcpy(bufExcess, (period+nFrames), bExcess);
	}
	memcpy(bufPosition, period, sizeof(short)*nFrames);

	bufPosition += nFrames;
	bLevel += nFrames;
	if(bLevel == bSize)
		bufferSwitch();
}

void RuAlsa::bufferSwitch() {
	bufLock.lock();

	if(cBuffer == 0)
		bufPosition = bufB;
	else
		bufPosition = bufA;
	bLevel = 0;
	bufLock.unlock();
	outsource(std::bind(&RuAlsa::actionFlushBuffer, this));
}

void RuAlsa::setConfig(string config, string value) {
	if(config == "latency") {
		mLatency = atoi(value.c_str());
		bSize = ((sampleRate<<1)/1000)*mLatency;
		cout << "Buffer: " << bSize << " frames in a period" << endl;
		bufA = (short*)calloc(bSize, sizeof(short));
		bufB = (short*)calloc(bSize, sizeof(short));
		bufExcess = (short*)calloc(bSize, sizeof(short));
		bufPosition = bufA;
	}
}

void RuAlsa::actionFlushBuffer() {
	bufLock.lock();
	short *period = NULL;

	if(cBuffer == 0) { 
		cBuffer++;
		period = bufA;
	}
	else {
		cBuffer--;
		period = bufB;
	}

	int frames;
	auto start = chrono::steady_clock::now();

	if((frames = snd_pcm_mmap_writei(handle, period, (bSize>>1) )) < (bSize>>1)) {
		if(frames == -EPIPE)
			cerr <<" Underrun occurred" << endl;
		else
			cerr << "Something went wrong ("<<frames<<")"<<endl;
	}
	auto end = chrono::steady_clock::now();
	auto diff = end - start;
	cout << chrono::duration <double, milli> (diff).count() << " ms" << " && " << frames << " Written" << endl;
	bufLock.unlock();

}

void RuAlsa::actionInitAlsa() {
	snd_pcm_hw_params_t *hw_params;
	int err, dir;
	unsigned int srate = 44100;
	snd_pcm_uframes_t nframes = 4096;

	if ((err = snd_pcm_open (&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		cerr << "cannot open audio device `default` - "
			<< snd_strerror(err) <<  endl;
		return;
	}
  
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		cerr << "cannot allocated hardware param struct - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0) {
		cerr << "cannot init hardware param struct - "
			<< snd_strerror(err) <<  endl;
		return;
	}

//	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_MMAP_INTERLEAVED)) < 0) {
		cerr << "cannot set access type - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_period_size_max (handle, hw_params, &nframes, &dir)) < 0) {
		cerr << "cannot set period max - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		cerr << "cannot set format - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, &srate, &dir)) < 0) {
		cerr << "cannot set sample rate - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, 2)) < 0) {
		cerr << "cannot set channels - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params (handle, hw_params)) < 0) {
		cerr << "cannot set parameters - "
			<< snd_strerror(err) <<  endl;

		return;
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (handle)) < 0) {
		cerr << "cannot prepare audio interface - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	snd_async_add_pcm_handler(&pcm_callback, handle,RuAlsaCallback,
			(void*)(new std::function<void()>(std::bind(&RuAlsa::asyncCallback, this))));

	int numFd = snd_pcm_poll_descriptors_count(handle);
	cout << numFd << " descriptors" << endl;
	cout << "RuAlsa: Initialised" << endl;
	
	workState = READY;
}

RackoonIO::RackState RuAlsa::init() {
	workState = INIT;
	outsource(std::bind(&RuAlsa::actionInitAlsa, this));
	return RACK_UNIT_OK;
}

RackoonIO::RackState RuAlsa::cycle() {
	if(workState < READY)
		return RACK_UNIT_OK;

	if(workState == READY)
		workState = STREAMING;

	if(workState == STREAMING) {
		return RACK_UNIT_OK;
	}

	return RACK_UNIT_OK;
}

void RuAlsa::asyncCallback() {
	cout << "Callback called" << endl;
}

void RuAlsaCallback(snd_async_handler_t *pcm_callback) {
	// run callback
	(*(std::function<void()>*)snd_async_handler_get_callback_private(pcm_callback))();
}
