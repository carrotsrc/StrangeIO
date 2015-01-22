#include "RuAlsa.h"

RuAlsa::RuAlsa()
: RackUnit() {
	addJack("audio", JACK_THREADED);
}

FeedState RuAlsa::feed(Jack *jack) {
	if(jack->name == "audio"){}
}

FeedState RuAlsa::feedJackAudio() {
	cout << "Thread Running" << endl;
	return FEED_OK;
}

void RuAlsa::setConfig(string config, string value) {
	
}

RackState RuAlsa::init() {

	snd_pcm_hw_params_t *hw_params;
	int err, dir;
	unsigned int srate = 44100;
	pthread_mutex_t *mutex;


	if ((err = snd_pcm_open (&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		cerr << "cannot open audio device `default` - "
			<< snd_strerror(err) <<  endl;
		return RACK_UNIT_FAILURE;
	}
  
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		cerr << "cannot allocated hardware param struct - "
			<< snd_strerror(err) <<  endl;
		return RACK_UNIT_FAILURE;
	}

	if ((err = snd_pcm_hw_params_any (handle, hw_params)) < 0) {
		cerr << "cannot init hardware param struct - "
			<< snd_strerror(err) <<  endl;
		return RACK_UNIT_FAILURE;
	}

	if ((err = snd_pcm_hw_params_set_access (handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		cerr << "cannot set access type - "
			<< snd_strerror(err) <<  endl;
		return RACK_UNIT_FAILURE;
	}

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		cerr << "cannot set format - "
			<< snd_strerror(err) <<  endl;
		return RACK_UNIT_FAILURE;
		return RACK_UNIT_FAILURE;
	}

	if ((err = snd_pcm_hw_params_set_rate_near (handle, hw_params, &srate, &dir)) < 0) {
		cerr << "cannot set sample rate - "
			<< snd_strerror(err) <<  endl;
		return RACK_UNIT_FAILURE;
	}

	if ((err = snd_pcm_hw_params_set_channels (handle, hw_params, 2)) < 0) {
		cerr << "cannot set channels - "
			<< snd_strerror(err) <<  endl;
		return RACK_UNIT_FAILURE;
	}

	if ((err = snd_pcm_hw_params (handle, hw_params)) < 0) {
		cerr << "cannot set parameters - "
			<< snd_strerror(err) <<  endl;

		return RACK_UNIT_FAILURE;
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (handle)) < 0) {
		cerr << "cannot prepare audio interface - "
			<< snd_strerror(err) <<  endl;
		return RACK_UNIT_FAILURE;
	}

	tAudio = new thread(&RuAlsa::feedJackAudio, this);

	return RACK_UNIT_OK;

}
