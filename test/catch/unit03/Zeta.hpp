#ifndef ZETA_HPP__
#define ZETA_HPP__
#ifdef __linux__

#include <thread>
#include <condition_variable>

#include <alsa/asoundlib.h>

#include "framework/component/unit.hpp" // Base class: strangeio::component::unit

class Zeta : public strangeio::component::unit
{
public:
	Zeta(std::string label);
	~Zeta();

public:
	strangeio::component::cycle_state cycle();
	void feed_line(strangeio::memory::cache_ptr samples, int line);
	strangeio::component::cycle_state init();
	void set_configuration(std::string key, std::string value);

private:
	// Buffer
	strangeio::memory::cache_ptr m_buffer;

	// Alsa variables
	snd_pcm_t *m_handle;
	snd_async_handler_t *m_cb;
	snd_pcm_uframes_t m_trigger_level, m_fperiod;
	unsigned int m_max_periods;

	// safe signal handling
	std::condition_variable m_signal_cv;
	std::mutex m_signal_mutex;
	std::thread* m_signal;
	bool m_running;

	void flush_samples();
};


#endif // linux check
#endif