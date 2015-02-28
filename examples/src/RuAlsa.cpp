/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "RuAlsa.h"
using namespace RackoonIO;
using namespace RackoonIO::Buffers;

RuAlsa::RuAlsa()
: RackUnit(std::string("RuAlsa")) {
	addJack("audio", JACK_SEQ);
	workState = IDLE;
	sampleRate = 44100;
	maxPeriods = 4;
	mLatency = 5;
	bufSize = 2048;
	bufLevel = 0;
	frameBuffer = nullptr;
}

RackoonIO::FeedState RuAlsa::feed(RackoonIO::Jack *jack) {

	Jack *j = getJack("audio");
	short *period;
	int bytes;

	if(frameBuffer->hasCapacity(j->frames) == DelayBuffer<short>::WAIT)
		return FEED_WAIT;


	if(j->flush(&period) == FEED_OK) {
		bufLock.lock();
		
		if(workState == PAUSED) {
			CONSOLE_MSG("RuAlsa", "Unpaused");
			workState = STREAMING;
		}
		frameBuffer->supply(period, j->frames);
		
		bufLock.unlock();
		
		cacheFree(period);
	}

	return FEED_OK;
}

void RuAlsa::setConfig(string config, string value) {
	if(config == "unit_buffer") {
		bufSize = (snd_pcm_uframes_t)atoi(value.c_str());
		frameBuffer = new DelayBuffer<short>(bufSize);
	} else if(config == "max_periods") {
		maxPeriods = atoi(value.c_str());
	}
}

void RuAlsa::actionFlushBuffer() {
	bufLock.lock();
	snd_pcm_uframes_t frames;
	int size = frameBuffer->getLoad();
	if((frames = snd_pcm_writei(handle, frameBuffer->flush(), (size>>1))) != (size>>1)) {
		if(frames == -EPIPE) {
			if(workState != PAUSED)
				cerr << "Underrun occurred" << endl;

			snd_pcm_recover(handle, frames, 0);
		}
		else
			cerr << "Something else is screwed" << endl;
	}

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
		frameBuffer = new Buffers::DelayBuffer<short>(bufSize);
		
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
		if(frameBuffer->getLoad() > 0 && currentLevel > triggerLevel) {
			workState = FLUSHING;
			outsource(std::bind(&RuAlsa::actionFlushBuffer, this));
		}

		return RACK_UNIT_OK;
	}

	if(workState == PRIMING && frameBuffer->getLoad() >= (fPeriod<<1)) {
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
	CONSOLE_MSG("RuAlsa", "Paused");
}
