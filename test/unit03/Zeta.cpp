#include <iostream>
#include <sstream>
#include "framework/alias.hpp"
#include "Zeta.hpp"
using namespace strangeio;
using namespace strangeio::component;

#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif

static void pcm_trigger_callback(snd_async_handler_t *);

Zeta::Zeta(std::string label)
	: strangeio::spec::dispatch("Zeta", label)
        , m_max_periods(3)
	, m_in_driver(0)
	, m_alsa_dev(std::string("default"))
	, m_running(false)
	, m_active(false)
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
	flush_samples();
}

void Zeta::flush_samples() {
	auto profile = global_profile();
	auto nframes = 0;
	// clear the held buffer
	{
		auto local_buffer = m_buffer;
		nframes = snd_pcm_writei(m_handle, local_buffer.get(), profile.period);
	}

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
	
	m_in_driver++;
	if(m_in_driver < 2) trigger_cycle();

	/* once it's done, set the unit back to streaming
	 * so the buffer continued to fill up
	 */
	//workState = STREAMING;
}

cycle_state Zeta::init() {


	snd_pcm_hw_params_t *hw_params;
	int err, dir = 0;
	std::stringstream ss;
	
	ss << "Device: " << m_alsa_dev << "";
	log(ss.str());
	ss.str("");

	if ((err = snd_pcm_open (&m_handle, m_alsa_dev.c_str(), SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0) {
		log("# cannot open audio device - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	auto num_fd = snd_pcm_poll_descriptors_count(m_handle);
    m_pfd = new struct pollfd[num_fd];
    num_fd = snd_pcm_poll_descriptors(m_handle, m_pfd, num_fd);

	if(!m_running) {

		m_signal = new std::thread([this, num_fd](){
			m_active = true;
			
			unsigned short rev;
			std::stringstream ss;
			ss << "Filled : " << num_fd << " descriptors";
			log(ss.str());
			ss.str("");

			while(1) {
				auto v = poll(m_pfd, num_fd, 5000);
				if(v < 0) {
					log("Poll error");
					continue;
				}

				snd_pcm_poll_descriptors_revents(m_handle, m_pfd, num_fd, &rev);
				if(rev & POLLOUT) {
					if(!m_running) {
						log("Toggle close down");
						break;
					}

					if(unit_profile().state == (int)line_state::inactive) {
						log("Inactive unit");
						// if inactive, don't bother triggering a cycle
						continue;
					}
					log("Writi")
					m_in_driver--;
					trigger_cycle();

				} else if(rev & POLLIN) {
					log("Data waiting to be read");
				} else if(rev & POLLERR) {
					log("IO Error");
				} else if(rev & POLLHUP) {
					log("Fd hung up");
				} else if(rev & POLLNVAL) {
					log("Descriptor not open");
				} else {
					//log("Unhandled revent");
				}
						

			}
			m_active = false;
		});

		m_running = true;
	}

	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		log("# cannot allocated hardware param struct - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_any (m_handle, hw_params)) < 0) {
		log("# cannot init hardware param struct - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_set_access (m_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		log("# cannot set access type - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params_set_format (m_handle, hw_params, SND_PCM_FORMAT_FLOAT_LE)) < 0) {
		log("# cannot set format - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	// Sample rate
	auto fs = 44100u;
	if ((err = snd_pcm_hw_params_set_rate_near (m_handle, hw_params, &fs, &dir)) < 0) {
		log("# cannot set sample rate - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	// Channels
	if ((err = snd_pcm_hw_params_set_channels (m_handle, hw_params, 2)) < 0) {
		log("# cannot set channels - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	auto min_period_size = 256ul, max_period_size = 2048ul;
	if(( err = 
		snd_pcm_hw_params_set_period_size_minmax(
			m_handle, hw_params,
			&min_period_size, &dir,
			&max_period_size, &dir
		)) < 0 ) {
		log("# cannot set minmax period size - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	ss << "min/max: " << min_period_size << "/" << max_period_size;
	log(ss.str());
	ss.str("");

	auto actual_period_size = min_period_size;

	if(m_cfg_period_size  > 0
	&& m_cfg_period_size >= min_period_size
	&& m_cfg_period_size <= max_period_size) {
		actual_period_size = m_cfg_period_size;
	}

	if ((err = snd_pcm_hw_params_set_period_size(m_handle, hw_params, actual_period_size, dir)) < 0) {
		ss << "# Cannot set period size to " << actual_period_size << " frames";
		log(ss.str());
		log("~ Setting to configurable minimum");
		actual_period_size = min_period_size;
		if ((err = snd_pcm_hw_params_set_period_size(m_handle, hw_params, min_period_size, dir)) < 0) {
			log("# Failed and bailed!");
			return cycle_state::error;
		}
	}


	if ((err = snd_pcm_hw_params_set_periods(m_handle, hw_params, 3, dir)) < 0) {
		log("# cannot set periods - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	if ((err = snd_pcm_hw_params (m_handle, hw_params)) < 0) {
		log("# cannot set parameters - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	snd_pcm_hw_params_free (hw_params);


	if ((err = snd_pcm_prepare (m_handle)) < 0) {
		log("# cannot prepare audio interface - ");
		log(std::string(snd_strerror(err)));
		return cycle_state::error;
	}

	auto period_size = 0ul;
	if ((err = snd_pcm_hw_params_get_period_size (hw_params, &period_size, &dir)) < 0) {
		log("# cannot get period size - ");
			log(std::string(snd_strerror(err)));
	}
	register_metric(profile_metric::period, (signed int)period_size);

	ss << "Period size : " << period_size << " frames";
	log(ss.str());
	ss.str("");


	snd_pcm_uframes_t buffer_size;
	if ((err = snd_pcm_hw_params_get_buffer_size (hw_params, &buffer_size)) < 0) {
		log("cannot get buffer size - ");
		log(std::string(snd_strerror(err)));
	}

	register_metric(profile_metric::latency, (signed int)(buffer_size/period_size));
	ss << "Buffer size : " << (buffer_size/period_size) << " periods (trigger on 1)";
	log(ss.str());
	ss.str("");


	auto sample_rate = 0u;
	if ((err = snd_pcm_hw_params_get_rate (hw_params, &sample_rate, &dir)) < 0) {
		log("cannot get sample rate - ");
		log(std::string(snd_strerror(err)));
	}
	register_metric(profile_metric::fs, (signed int)sample_rate);


	//	Handle async signals safely
	auto *func = new std::function<void(void)>([this](){
		//std::cout << m_in_driver << "in driver" << std::endl;
		if(unit_profile().state == (int)line_state::active) m_in_driver--;
		m_signal_cv.notify_one();
	});

	register_metric(profile_metric::state, (signed int)line_state::active);
	log("Initialised");
	return cycle_state::complete;
}

std::string Zeta::get_configuration(std::string key, std::string value) {
	return std::string();
}

void Zeta::set_configuration(std::string key, std::string value) {
	if(key == "device") {
		m_alsa_dev = value;
	}
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
        std::cout << "CB" << std::endl;
	auto callback = (std::function<void(void)>*)snd_async_handler_get_callback_private(cb);
	(*callback)();
}

UnitBuilder(Zeta);