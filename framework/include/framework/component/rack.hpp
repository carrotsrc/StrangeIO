#ifndef __RACK_HPP_1440421209__
#define __RACK_HPP_1440421209__

#include <map>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "framework/component/component.hpp"
#include "framework/component/backend.hpp"
#include "framework/component/utility_container.hpp"


namespace strangeio {
namespace component {


class rack 
	: public backend
	, public utility_container
{
public:
	rack();
	~rack();


	// Communication
	void trigger_sync(sync_flag flags = 0);
	void trigger_cycle();

	// Control
	void warmup();
	void start();
	void stop();

	// State
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

