#include "RuAlsa.h"
using namespace RackoonIO;

RuAlsa::RuAlsa()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	workState = IDLE;
}

RackoonIO::FeedState RuAlsa::feed(RackoonIO::Jack *jack) {
	Jack *j = getJack("audio");
	short *period;
	int bytes;
	if(j->flush(&period) == FEED_OK) {
		if((bytes = snd_pcm_writei(handle, period, (0x100>>1))) < (0x100>>1)) {
			if(bytes == -EPIPE)
				cout <<" Underrun occurred" << endl;
		}
		free(period);
	}

	return FEED_OK;
}

void RuAlsa::setConfig(string config, string value) {
	
}

void RuAlsa::actionInitAlsa() {
	snd_pcm_hw_params_t *hw_params;
	int err, dir;
	unsigned int srate = 44100;

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
