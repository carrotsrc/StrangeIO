#include "RuAlsa.h"
using namespace RackoonIO;

RuAlsa::RuAlsa()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	workState = IDLE;
	sampleRate = 44100;
	maxPeriod = 4;
	mLatency = 5;
	bufSize = 2048;
	bufLevel = 0;
	frameBuffer = nullptr;

}

RackoonIO::FeedState RuAlsa::feed(RackoonIO::Jack *jack) {
	Jack *j = getJack("audio");
	short *period;
	int bytes;

	if(j->frames + bufLevel > bufSize)
		return FEED_WAIT;


	if(j->flush(&period) == FEED_OK) {

		bufLock.lock();
		memcpy(frameBuffer+bufLevel, period, j->frames);
		bufLevel += j->frames;
		bufLock.unlock();
		free(period);
	}

	return FEED_OK;
}

void RuAlsa::setConfig(string config, string value) {
	if(config == "period") {
		fPeriod = (snd_pcm_uframes_t)atoi(value.c_str());
	} else if(config == "unit_buffer") {
		bufSize = (snd_pcm_uframes_t)atoi(value.c_str());
		frameBuffer = (short*)malloc(sizeof(short)*bufSize);
		if(frameBuffer == NULL)
			cerr << "Failed to allocate frame buffer" << endl;
	} else if(config == "max_periods") {
		maxPeriod = atoi(value.c_str());
	}
}

void RuAlsa::actionFlushBuffer() {
	bufLock.lock();
	snd_pcm_uframes_t frames;
	if((frames = snd_pcm_writei(handle, frameBuffer, (bufLevel>>1))) < (bufLevel>>1)) {
		if(frames == -EPIPE)
			cerr << "Underrun occurred" << endl;
		else
			cerr << "Something else is fucked" << endl;
	} else
		cout << bufLevel << endl;
	fwrite(frameBuffer, sizeof(short), bufLevel, fp);
	bufLevel = 0;
	bufLock.unlock();
	workState = STREAMING;
}

void RuAlsa::actionInitAlsa() {
	fp = fopen("Test.raw", "wb");
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_uframes_t maxPeriodSize;
	int err, dir;
	unsigned int srate = 44100;
	dir = 1;

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

	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
//	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_MMAP_INTERLEAVED)) < 0) {
		cerr << "cannot set access type - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_period_size (handle, hw_params, fPeriod, dir)) < 0) {
		cerr << "cannot set period size - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_get_period_size (hw_params, &fPeriod, &dir)) < 0) {
		cerr << "cannot get period size - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	cout << "Period Size: " << fPeriod << endl;

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		cerr << "cannot set format - "
			<< snd_strerror(err) <<  endl;
		return;
	}

/*	
	if ((err = snd_pcm_hw_params_set_rate_max (handle, hw_params, &srate, &dir)) < 0) {
		cerr << "cannot set sample rate - "
			<< snd_strerror(err) <<  endl;
		return;
	}
*/

	if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, 2)) < 0) {
		cerr << "cannot set channels - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_periods_max(handle, hw_params, &maxPeriod, &dir)) < 0) {
		cerr << "cannot set periods - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_periods(handle, hw_params, maxPeriod, dir)) < 0) {
		cerr << "cannot set periods - "
			<< snd_strerror(err) <<  endl;
		return;
	}
 	snd_pcm_hw_params_get_periods(hw_params, &maxPeriod, &dir);
	cout << "Periods per buffer: " << maxPeriod << endl;


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
	snd_pcm_uframes_t bsz;
	triggerLevel = snd_pcm_avail_update(handle);
	snd_pcm_hw_params_get_buffer_size(hw_params, &bsz);
	cout << "Buffer size: " << bsz << endl;

	if ((err = snd_pcm_hw_params_get_rate (hw_params, &srate, &dir)) < 0) {
		cerr << "cannot set sample rate - "
			<< snd_strerror(err) <<  endl;
		return;
	}
	cout << "Rate : " << srate << endl;

	triggerLevel -= (fPeriod<<1);
	cout << "Trigger Pointer: " << triggerLevel << endl;

	if(frameBuffer == nullptr)
		frameBuffer = (short*)malloc(sizeof(short)*bufSize);



	/*snd_async_add_pcm_handler(&pcm_callback, handle, RuAlsaCallback,
			(void*)(new std::function<void()>(std::bind(&RuAlsa::asyncCallback, this))));*/
	cout << "RuAlsa: Initialised" << endl;
	
	workState = READY;
}

RackoonIO::RackState RuAlsa::init() {
	workState = INIT;
	outsource(std::bind(&RuAlsa::actionInitAlsa, this));
	return RACK_UNIT_OK;
}

RackoonIO::RackState RuAlsa::cycle() {
	snd_pcm_sframes_t currentLevel;
	if(workState == STREAMING) {
		currentLevel = snd_pcm_avail_update(handle);
		if(bufLevel > 0 && currentLevel > triggerLevel) {
			workState = FLUSHING;
			outsource(std::bind(&RuAlsa::actionFlushBuffer, this));
		}

		return RACK_UNIT_OK;
	}

	if(workState == PRIMING && bufLevel >= (fPeriod<<1)) {
		cout << "Switched to STREAMING" << endl;
		workState = STREAMING;
	}

	if(workState < READY)
		return RACK_UNIT_OK;

	if(workState == READY) {
		workState = PRIMING;
	}


	return RACK_UNIT_OK;
}

/*void RuAlsa::asyncCallback() {
	cout << "ready to be written: " << frames_to_deliver << endl;
}

void RuAlsaCallback(snd_async_handler_t *pcm_callback) {
	// run callback
	(*(std::function<void()>*)snd_async_handler_get_callback_private(pcm_callback))();
}*/
