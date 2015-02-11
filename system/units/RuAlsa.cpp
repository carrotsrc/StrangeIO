#include "RuAlsa.h"
#include "system/events/ShellEvents.h"
using namespace RackoonIO;

RuAlsa::RuAlsa()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	workState = IDLE;
	sampleRate = 44100;
	maxPeriods = 4;
	mLatency = 5;
	bufSize = 2048;
	bufLevel = 0;
	frameBuffer = nullptr;
	fp = fopen("pcm.raw", "wb");
}

RackoonIO::FeedState RuAlsa::feed(RackoonIO::Jack *jack) {
	Jack *j = getJack("audio");
	short *period;
	int bytes;

	if(j->frames + bufLevel > bufSize)
		return FEED_WAIT;

	if(j->flush(&period) == FEED_OK) {
		bufLock.lock();
		if(workState == PAUSED) {
			cout << "RuAlsa: Unpaused" << endl;
			workState = STREAMING;
		}
		memcpy(frameBuffer+bufLevel, period, (j->frames*sizeof(short)));
		bufLevel += j->frames;
		bufLock.unlock();
		free(period);
	}

	return FEED_OK;
}

void RuAlsa::setConfig(string config, string value) {
	if(config == "unit_buffer") {
		bufSize = (snd_pcm_uframes_t)atoi(value.c_str());
		frameBuffer = (short*)malloc(sizeof(short)*bufSize);
		if(frameBuffer == NULL)
			cerr << "Failed to allocate frame buffer" << endl;
	} else if(config == "max_periods") {
		maxPeriods = atoi(value.c_str());
	}
}

void RuAlsa::actionFlushBuffer() {
	bufLock.lock();
	snd_pcm_uframes_t frames;
	if((frames = snd_pcm_writei(handle, frameBuffer, (bufLevel>>1))) != (bufLevel>>1)) {
		if(frames == -EPIPE) {
			if(workState != PAUSED)
				cerr << "Underrun occurred" << endl;

			snd_pcm_recover(handle, frames, 0);
		}
		else
			cerr << "Something else is fucked" << endl;
	}
	fwrite(frameBuffer, sizeof(short), bufLevel, fp);
	bufLevel = 0;
	bufLock.unlock();
	if(workState == PAUSED)
		return;

	workState = STREAMING;
}

void RuAlsa::actionInitAlsa() {
	snd_pcm_hw_params_t *hw_params;
	int err, dir = 0;

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
		cerr << "cannot set access type - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		cerr << "cannot set format - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	
	if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, &sampleRate, &dir)) < 0) {
		cerr << "cannot set sample rate - "
			<< snd_strerror(err) <<  endl;
	}


	if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, 2)) < 0) {
		cerr << "cannot set channels - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_periods_max(handle, hw_params, &maxPeriods, &dir)) < 0) {
		cerr << "cannot set periods - "
			<< snd_strerror(err) <<  endl;
		return;
	}

	if ((err = snd_pcm_hw_params_set_periods(handle, hw_params, maxPeriods, dir)) < 0) {
		cerr << "cannot set periods - "
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

	if ((err = snd_pcm_hw_params_get_period_size (hw_params, &fPeriod, &dir)) < 0) {
		cerr << "cannot get period size - "
			<< snd_strerror(err) <<  endl;
	}

	cout << "RuAlsa: Period size: " << fPeriod << endl;

	snd_pcm_uframes_t bsize;
	if ((err = snd_pcm_hw_params_get_buffer_size (hw_params, &bsize)) < 0) {
		cerr << "cannot get sample rate - "
			<< snd_strerror(err) <<  endl;
	}

	cout << "RuAlsa: Buffer Size: " << bsize << endl;

	if ((err = snd_pcm_hw_params_get_rate (hw_params, &sampleRate, &dir)) < 0) {
		cerr << "cannot get sample rate - "
			<< snd_strerror(err) <<  endl;
	}

	cout << "RuAlsa: Sample rate: " << sampleRate << endl;

	triggerLevel = snd_pcm_avail_update(handle) - (fPeriod<<1);

	if(frameBuffer == nullptr)
		frameBuffer = (short*)malloc(sizeof(short)*bufSize);

	CONSOLE_MSG("RuAlsa", "Initialised");
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
		workState = STREAMING;
	}

	if(workState < READY)
		return RACK_UNIT_OK;

	if(workState == READY) {
		workState = PRIMING;
	}


	return RACK_UNIT_OK;
}

void RuAlsa::block(Jack *jack) {
	workState = PAUSED;
	cout << "RuAlsa: Paused" << endl;
}
