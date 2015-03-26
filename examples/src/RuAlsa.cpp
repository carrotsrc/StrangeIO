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
#include "framework/events/FrameworkMessages.h"
using namespace RackoonIO;
using namespace RackoonIO::Buffers;
using namespace ExampleCode;


static void pcm_trigger_callback(snd_async_handler_t *);
/** Set the default values */
RuAlsa::RuAlsa()
: RackUnit(std::string("RuAlsa")) {
	addJack("audio", JACK_SEQ);
	workState = IDLE;
	sampleRate = 44100;
	maxPeriods = 4;
	bufSize = 2048;
	frameBuffer = nullptr;
	//fp = fopen("pcm.raw", "wb");
}

/** Method that is called when there is data waiting to be fed into the unit
 *
 * Here we decide whether to accept the data and store it in the buffer
 * or respond with a FEED_WAIT
 */
RackoonIO::FeedState RuAlsa::feed(RackoonIO::Jack *jack) {
	PcmSample *period;

	// here the buffer has reached capacity
	if(frameBuffer->hasCapacity(jack->frames) == DelayBuffer<PcmSample>::WAIT)
		return FEED_WAIT; // so response with a WAIT


	// If we're here then the buffer has room
	if(jack->flush(&period) == FEED_OK) {
		bufLock.lock();
		
		if(workState == PAUSED) {
			CONSOLE_MSG("RuAlsa", "Unpaused");
			workState = STREAMING;
		}
		frameBuffer->supply(period, jack->frames);
		bufLock.unlock();
		cacheFree(period); // We are freeing the block of cache
		notifyProcComplete();
	}

	return FEED_OK; // We've accepted the period
}
/** Method for setting configurations
 *
 * This unit accepts:
 * 	- unit_buffer : The size of the buffer (in frames)
 * 	- max_periods : The maximum periods that can be held by ASLA
 */
void RuAlsa::setConfig(string config, string value) {
	if(config == "unit_buffer") {
		bufSize = (snd_pcm_uframes_t)atoi(value.c_str());
		frameBuffer = new DelayBuffer<PcmSample>(bufSize);
	} else if(config == "max_periods") {
		maxPeriods = atoi(value.c_str());
	}
}

/** This is am outsourced method for flushing the delay buffer
 *
 * This is the task that writes the delay buffer into ALSA
 */
void RuAlsa::actionFlushBuffer() {
	bufLock.lock();
	snd_pcm_uframes_t nFrames;
	int size = frameBuffer->getLoad();
	const PcmSample *frames = frameBuffer->flush();
	if((nFrames = snd_pcm_writei(handle, frames, (size>>1))) != (size>>1)) {
		if(nFrames == -EPIPE) {
			if(workState != PAUSED)
				cerr << "Underrun occurred" << endl;

			snd_pcm_recover(handle, nFrames, 0);
		}
		else
			cerr << "Something else is screwed" << endl;
	}
	//fwrite(frames, sizeof(PcmSample), size, fp);
	bufLock.unlock();
	notifyProcComplete();
	if(workState == PAUSED)
		return;

	/* once it's done, set the unit back to streaming
	 * so the buffer continued to fill up
	 */
	workState = STREAMING;
}

/** Intialise ALSA
 *
 * This method is outsourced so the initialisation
 * can happen in parallel to the rest of the rack
 * cycle. This frees up the rack cycle but also
 * means we need to be more careful about what
 * state we are in
 */
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

	if ((err = snd_pcm_hw_params_set_format (handle, hw_params, SND_PCM_FORMAT_FLOAT_LE)) < 0) {
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

	UnitMsg("Period size: " << fPeriod);

	snd_pcm_uframes_t bsize;
	if ((err = snd_pcm_hw_params_get_buffer_size (hw_params, &bsize)) < 0) {
		cerr << "cannot get sample rate - "
			<< snd_strerror(err) <<  endl;
	}

	UnitMsg("Buffer Size: " << bsize);

	if ((err = snd_pcm_hw_params_get_rate (hw_params, &sampleRate, &dir)) < 0) {
		cerr << "cannot get sample rate - "
			<< snd_strerror(err) <<  endl;
	}

	UnitMsg("Fs: " << sampleRate);

	triggerLevel = snd_pcm_avail_update(handle) - (fPeriod<<1);

	if(frameBuffer == nullptr)
		frameBuffer = new Buffers::DelayBuffer<PcmSample>(bufSize);
		
	auto *func = new std::function<void(void)>(std::bind(&RuAlsa::triggerAction, this));
	snd_async_add_pcm_handler(&cb, handle, &pcm_trigger_callback, (void*)func);
	UnitMsg("Initialised");
	notifyProcComplete();
	workState = READY;
}

/** Intitialise the Unit
 *
 * This is called on the warm-up cycle.
 * We don't initialise the unit here,
 * but outsource the task
 */
RackoonIO::RackState RuAlsa::init() {

	/* Set to INIT because the unit
	 * won't be ready by the end
	 * of the method. It will still
	 * be initialising
	 */
	workState = INIT;

	/* Here the task it outsourced to the threadpool
	 * which means the rack cycle is not being blocked
	 */

	OUTSRC(RuAlsa::actionInitAlsa);
	/* ^^^^ that is a macro which expands to this:
	 * 
	 * outsource(std::bind(&RuAlsa::actionInitAlsa, this));
	 */

	return RACK_UNIT_OK;
}

/** The method on the rack cycle (After the warm-up cycle)
 *
 * It is important to keep track of the state since it
 * will be changed by parallel tasks in another thread
 */
RackoonIO::RackState RuAlsa::cycle() {
	snd_pcm_sframes_t currentLevel;
	if(workState == STREAMING) {
		currentLevel = snd_pcm_avail_update(handle);
		// Check to see if ALSA has reached the threshold
		if(frameBuffer->getLoad() > 0 && currentLevel > triggerLevel) {
			// ALSA is running out of frames! Trigger a flush
			workState = FLUSHING; // Change state

			// outsource the flushing task
			outsource(std::bind(&RuAlsa::actionFlushBuffer, this));
		}

		return RACK_UNIT_OK;
	}

	if(workState == PRIMING && frameBuffer->getLoad() >= (fPeriod<<1)) {
		/* Here the delay buffer has been primed 
		 * and ready to start feeding to alsa
		 */
		workState = STREAMING;
	}

	if(workState < READY)
		return RACK_UNIT_OK;

	if(workState == READY) {
		/* The unit has been initialised
		 * (by the other thread) and so
		 * it is time to start priming 
		 * the delay buffer.
		 */
		workState = PRIMING;
	}


	return RACK_UNIT_OK;
}

void RuAlsa::block(Jack *jack) {
	workState = PAUSED;
	CONSOLE_MSG("RuAlsa", "Paused");
}

void RuAlsa::triggerAction() {
	outsource(std::bind(&RuAlsa::actionFlushBuffer, this));
}

static void pcm_trigger_callback(snd_async_handler_t *cb) {
	auto callback = (std::function<void(void)>*)snd_async_handler_get_callback_private(cb);
	(*callback)();
}
