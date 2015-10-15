#ifndef __RACK_HPP_1440421209__
#define __RACK_HPP_1440421209__

#include <map>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "framework/component/component.hpp"
#include "framework/component/backend.hpp"
#include "framework/midi/midi_handler.hpp"

#include "framework/memory/cache_utility.hpp"
#include "framework/thread/queue_utility.hpp"
#include "framework/event/loop_utility.hpp"

namespace strangeio {
namespace component {


class rack 
	: public backend {
public:
	rack();
	~rack();

	// Utilities
	void set_cache_utility(memory::cache_utility* cache);
	memory::cache_utility* get_cache_utility();
	
	void set_queue_utility(thread::queue_utility* queue);
	thread::queue_utility* get_queue_utility();
	
	void set_loop_utility(event::loop_utility* loop);
	event::loop_utility* get_loop_utility();

	void set_midi_handler(midi::midi_handler* midi);
	midi::midi_handler* get_midi_handler();

	// Communication
	void trigger_sync(sync_flag flags = 0);
	void trigger_cycle();

	// Control
	void warmup();
	void start();
	void stop();

	bool running();
	bool active();

protected:
	void resync();
	void mount_dependencies(unit* u);

private:
	std::thread m_rack_thread;
	std::condition_variable m_trigger;
	std::mutex m_trigger_mutex;
	volatile bool m_active, m_running;

	memory::cache_utility* m_cache;
	thread::queue_utility* m_queue;
	event::loop_utility* m_loop;
	midi::midi_handler* m_midi;

	std::atomic<bool> m_resync;
	sync_flag m_resync_flags;
	std::atomic<int> m_cycle_queue;
	
	


};

using rack_uptr = std::unique_ptr<strangeio::component::rack>;
using rack_sptr = std::shared_ptr<strangeio::component::rack>;
using rack_wptr = std::weak_ptr<strangeio::component::rack>;

} // Component
} // StrangeIO
#endif

