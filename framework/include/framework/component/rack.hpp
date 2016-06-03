#ifndef __RACK_HPP_1440421209__
#define __RACK_HPP_1440421209__

#include <map>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "framework/component/component.hpp"
#include "framework/component/backend.hpp"
#include "framework/component/utility_container.hpp"
#include "framework/thread/scheduled.hpp"


namespace strangeio {
namespace component {

/**
 * The central controller of the system
 * 
 * The rack is the main object and the primary composition of classes. It acts 
 * as driver for cycling the units that are hooked into it's daisychains.
 */
class rack 
	: public backend
	, public utility_container
{
public:
	rack();
	~rack();


	// Communication
	
	/**
	 * Notify rack to run a sync at the end of the cycle
	 * 
	 * If different syncs are triggered in the same cycle, they will all be run
	 * in priority order.
	 * 
	 * @param flags A bitfield of types of syncs to trigger
	 */
	void trigger_sync(sync_flag flags = 0);
	
	/**
	 * Notify the rack to trigger a cycle at the end of currentcycle
	 * 
	 * Triggering more than one cycle will compound the number of queued cycles
	 */
	void trigger_cycle();

	// Control
	
	/**
	 * Warm up the daisychains of units
	 */
	void warmup();
	
	/**
	 * Start the rack worker thread
	 */
	void start();
	
	/**
	 * Notify the rack worker thread to stop
	 */
	void stop();

	/**
	 * Check whether the rack is in a running state
	 * 
	 * @return true if running; otherwise false.
	 */
	bool running();
	
	/**
	 * Check whether the rack worker thread is active
	 * 
	 * @return true if active; otherwise false.
	 */
	bool active();
	
	// policy
	/**
	 * Assign the policy to the thread
	 * @param policy Policy type
	 */
	void assign_schpolicy(strangeio::thread::sched_desc policy);

protected:
	
	/**
	 * Run on a global sync
	 */
	void resync();
	
	/**
	 * Set the dependencies in a unit
	 * 
	 * @param u Pointer to unit that is receiving deps
	 */
	void mount_dependencies(unit* u);

private:
	
	strangeio::thread::sched_desc m_schpolicy;
	strangeio::thread::scheduled m_rack_thread;
	std::condition_variable m_trigger;
	std::mutex m_trigger_mutex;
	volatile bool m_active, m_running;

	

	std::atomic<bool> m_resync;
	sync_flag m_resync_flags;
	std::atomic<int> m_cycle_queue;
	strangeio::routine::debug::tp m_tps, m_tpe;
        long int m_last_trigger, m_thread_trig, m_thread_resume;
};

using rack_uptr = std::unique_ptr<strangeio::component::rack>;
using rack_sptr = std::shared_ptr<strangeio::component::rack>;
using rack_wptr = std::weak_ptr<strangeio::component::rack>;

} // Component
} // StrangeIO
#endif

