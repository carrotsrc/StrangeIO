#include <iostream>

#include "Zeta.hpp"
using namespace strangeio;
using namespace strangeio::component;


static void pcm_trigger_callback(snd_async_handler_t *);

Zeta::Zeta(std::string label)
	: unit(unit_type::dispatch, "Zeta", label)
{
	add_input("audio_in");
}

Zeta::~Zeta() {

}

cycle_state Zeta::cycle() {

	return cycle_state::complete;
}

void Zeta::feed_line(memory::cache_ptr samples, int line) {
	m_buffer = samples;
}

void Zeta::flush_samples() {
	auto profile = global_profile();
	auto nframes = snd_pcm_writei(m_handle, m_buffer.release(), profile.period);
	if(nframes != (signed) profile.period) {
		if(nframes == -EPIPE) {
//			if(workState != PAUSED)
			std::cerr << "Underrun occurred" << std::endl;
			snd_pcm_recover(m_handle, nframes, 0);

		} else {
			std::cerr << "Screwed: Code[" << (signed int)nframes << "]" << std::endl;
			std::cerr << snd_strerror(nframes) << std::endl;
			snd_pcm_recover(m_handle, nframes, 0);
		}
	}


	/* once it's done, set the unit back to streaming
	 * so the buffer continued to fill up
	 */
	//workState = STREAMING;
}

cycle_state Zeta::init() {

	snd_pcm_hw_params_t *hw_params;
	int err, dir = 0;

	if(!m_running) {

		m_signal = new std::thread([this](){

			std::unique_lock<std::mutex> ul(m_signal_mutex);

			while(1) {
				m_signal_cv.wait(ul);
				if(!m_running) {
					ul.unlock();
					break;
				}
				flush_samples();
			}

		});

		m_running = true;
	}


	if ((err = snd_pcm_open (&m_handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		log("cannot open audio device `default` - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		log("cannot allocated hardware param struct - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_any (m_handle, hw_params)) < 0) {
		log("cannot init hardware param struct - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_set_access (m_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		log("cannot set access type - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_set_format (m_handle, hw_params, SND_PCM_FORMAT_FLOAT_LE)) < 0) {
		log("cannot set format - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	// Sample rate
	auto fs = 44100u;
	if ((err = snd_pcm_hw_params_set_rate_near (m_handle, hw_params, &fs, &dir)) < 0) {
		log("cannot set sample rate - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	// Channels
	if ((err = snd_pcm_hw_params_set_channels (m_handle, hw_params, 2)) < 0) {
		log("cannot set channels - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	// Period sizes
	auto min_period_size = 0ul;
	if ((err = snd_pcm_hw_params_get_period_size_min(hw_params, &min_period_size, &dir)) < 0) {
		log("cannot get period size - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_set_period_size(m_handle, hw_params, min_period_size, dir)) < 0) {
		log("cannot set period size - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_set_periods_max(m_handle, hw_params, &m_max_periods, &dir)) < 0) {
		log("cannot set periods - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_set_periods(m_handle, hw_params, 2, dir)) < 0) {
		log("cannot set periods - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params (m_handle, hw_params)) < 0) {
		log("cannot set parameters - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (m_handle)) < 0) {
		log("cannot prepare audio interface - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	auto period_size = 0ul;
	if ((err = snd_pcm_hw_params_get_period_size (hw_params, &period_size, &dir)) < 0) {
		log("cannot get period size - ");
			log(std::string(snd_strerror(err)));
	}
	register_metric(profile_metric::period, (signed int)period_size);

	snd_pcm_uframes_t buffer_size;
	if ((err = snd_pcm_hw_params_get_buffer_size (hw_params, &buffer_size)) < 0) {
		log("cannot get buffer size - ");
		log(std::string(snd_strerror(err)));
	}
	register_metric(profile_metric::latency, (signed int)buffer_size);
	

	auto sample_rate = 0u;
	if ((err = snd_pcm_hw_params_get_rate (hw_params, &sample_rate, &dir)) < 0) {
		log("cannot get sample rate - ");
		log(std::string(snd_strerror(err)));
	}
	register_metric(profile_metric::fs, (signed int)sample_rate);


	m_trigger_level = snd_pcm_avail_update(m_handle) - (m_fperiod<<1);

	//	Handle async signals safely
	auto *func = new std::function<void(void)>([this](){
		m_signal_cv.notify_one();
	});
	snd_async_add_pcm_handler(&m_cb, m_handle, &pcm_trigger_callback, (void*)func);

	log("Initialised");
	return cycle_state::complete;
}

void Zeta::set_configuration(std::string key, std::string value) {

}

/* This is a signal handler. ALSA uses SIGIO to tell the unit that the
 * output buffer is running low and needs to be refilled. This function
 * is executed when the signal is received. The signal is async and is
 * sent to the main process thread. It is important that there are no heap
 * allocations that occur in the callstack from this function because
 * if a heap allocation in the main process is interrupted by the
 * signal, and another heap allocation occurs somewhere from the signal
 * handler there will be a deadlock on __lll_lock_wait_private.
 *
 * No:
 * [m/c/re]alloc()
 * free()
 * new
 * delete
 */
static void pcm_trigger_callback(snd_async_handler_t *cb) {
	auto callback = (std::function<void(void)>*)snd_async_handler_get_callback_private(cb);
	(*callback)();
}

UnitBuilder(Zeta);