#ifndef ZETA_HPP__
#define ZETA_HPP__
#ifdef __linux__

#include <poll.h>

#include <thread>
#include <atomic>
#include <condition_variable>

#include <alsa/asoundlib.h>


#include "framework/spec/dispatch.hpp"


class Zeta : public strangeio::spec::dispatch
{
public:
	Zeta(std::string label);
	~Zeta();

public:
	strangeio::component::cycle_state cycle();
	void feed_line(strangeio::memory::cache_ptr samples, int line);
	strangeio::component::cycle_state init();
	std::string get_configuration(std::string key, std::string value);
        void set_configuration(std::string key, std::string value);

private:
	// Buffer
	strangeio::memory::cache_ptr m_buffer;

	// Alsa variables
	snd_pcm_t *m_handle;
	snd_async_handler_t *m_cb;
	snd_pcm_uframes_t m_trigger_level, m_fperiod;
	unsigned int m_max_periods;
	unsigned int m_cfg_period_size;
        int m_in_driver;
        
        struct pollfd *m_pfd;
        std::string m_alsa_dev;
        

	// safe signal handling
	std::condition_variable m_signal_cv;
	std::mutex m_signal_mutex;
	std::thread* m_signal;
	bool m_running, m_active;

	void flush_samples();
};


#endif // linux check
#endif