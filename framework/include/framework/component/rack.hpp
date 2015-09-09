#ifndef __RACK_HPP_1440421209__
#define __RACK_HPP_1440421209__

#include <map>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "framework/component/component.hpp"
#include "framework/component/unit.hpp"
#include "framework/midi/midi_handler.hpp"

#include "framework/memory/cache_utility.hpp"
#include "framework/thread/queue_utility.hpp"
#include "framework/event/loop_utility.hpp"

namespace strangeio {
namespace component {

struct rack_profile {
	profile_duration sync_duration;
	profile_duration cycle_duration;
};

class rack : public rack_utility {
public:
	rack();
	~rack();

	// Connections
	void add_mainline(std::string name);
	bool connect_mainline(std::string mainline, std::string unit);
	bool connect_units(std::string from, std::string out, std::string to, std::string in);

	// Units
	void add_unit(unit_uptr unit);
	const std::map<std::string, unit_sptr> & get_units();
	unit_wptr get_unit(std::string label);
	bool has_unit(std::string label);
	void clear_units();

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

	void sync(sync_flag flags);
	void sync(sync_profile& profile, sync_flag flags);
	cycle_state cycle(cycle_type type = cycle_type::ac);

	// Profile stats
	const rack_profile& profile();
	const sync_profile& global_profile();
	bool profile_line(component::sync_profile& profile, std::string mainline);

	// Control
	void warmup();
	void start();
	void stop();

	bool running();
	bool active();

protected:
	
private:
	std::map<std::string, unit*> m_mainlines;
	std::map<std::string, unit_sptr> m_mounted;
	std::vector<unit*> m_raw_mainlines;

	std::thread m_rack_thread;
	std::condition_variable m_trigger;
	std::mutex m_trigger_mutex;
	volatile bool m_active, m_running;

	memory::cache_utility* m_cache;
	thread::queue_utility* m_queue;
	event::loop_utility* m_loop;
	midi::midi_handler* m_midi;

	rack_profile m_rack_profile;
	sync_profile m_global_profile;

	std::atomic<bool> m_resync;
	sync_flag m_resync_flags;
	std::atomic<int> m_cycle_queue;

	// Profile methods
	void profile_sync(sync_flag flags);
	void sync_cache();
};

using rack_uptr = std::unique_ptr<strangeio::component::rack>;
using rack_sptr = std::shared_ptr<strangeio::component::rack>;
using rack_wptr = std::weak_ptr<strangeio::component::rack>;

} // Component
} // StrangeIO
#endif

