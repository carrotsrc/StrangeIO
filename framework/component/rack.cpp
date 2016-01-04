#include <chrono>
#include <iostream>

#ifdef __linux__
	#include <sched.h>
	#include <errno.h>
	#include <string.h>
	#define	SIO_SCHED SCHED_FIFO
#endif


#define PATH_NONE 0
#define PATH_CYCLE 1
#define PATH_WAKEUP 2

//#define DEBUG_PATH (PATH_CYCLE | PATH_WAKEUP)
//#define DEBUG_PATH PATH_WAKEUP
#define DEBUG_PATH PATH_NONE


#include "framework/alias.hpp"
#include "framework/routine/debug.hpp"
#include "framework/component/rack.hpp"

using namespace strangeio;
using namespace strangeio::component;

using pclock = std::chrono::steady_clock;

#if DEBUG_PATH & PATH_WAKEUP
static siortn::debug::tp dbg_wus, dbg_wue;
#endif



rack::rack()
	: backend()
	, m_schpolicy({0})
	, m_active(false)
	, m_running(false)
	, m_resync(false)
	, m_resync_flags(0)
	, m_cycle_queue(0)
	, m_last_trigger(0)
{ }

rack::~rack() {
	if(m_active) {
		stop();
	}
	
	if(m_cache) delete m_cache;
}


void rack::mount_dependencies(unit* u) {
    
	u->set_rack(this);

	u->set_cache_utility(m_cache);
	u->set_loop_utility(m_loop);
	u->set_queue_utility(m_queue);
}

void rack::trigger_sync(sync_flag flags) {
	// don't unflag previous flags
	if(flags) m_resync_flags |= flags;
	m_resync = true;
}

void rack::trigger_cycle() {
	++m_cycle_queue;

#if DEBUG_PATH & PATH_WAKEUP
dbg_wus = siortn::debug::clock_time();
#endif

	m_trigger.notify_one();
}

void rack::resync() {
	if(m_cache == nullptr) return;
	if(m_cache->block_size() > 0) return;
	
	m_cache->build_cache((m_global_profile.period*2) * m_global_profile.channels);
}



void rack::warmup() {
	cycle(cycle_type::warmup);
	cycle(cycle_type::sync);
	sync((sync_flag)sync_flags::glob_sync);
}

#include <chrono>
#include <limits>

void rack::start() {
	m_running = true;
	m_active = false;
	
	
	m_rack_thread = siothr::scheduled(m_schpolicy, [this](){

		// ---------

		m_active = true;
		std::unique_lock<std::mutex> lock(m_trigger_mutex);

		while(m_running) {
			m_trigger.wait(lock);			

#if DEBUG_PATH & PATH_WAKEUP
dbg_wue = strangeio::routine::debug::clock_time();
auto wu_diff = siortn::debug::clock_delta_us(dbg_wus, dbg_wue);
if(wu_diff > 150) {
std::cout << "r: "
<< siortn::debug::clock_delta_us(dbg_wus, dbg_wue)
<< std::endl;
}
#endif			
			
			if(!m_running) {
				lock.unlock();
				break;
			}

			while(m_cycle_queue > 0) {

#if DEBUG_PATH & PATH_CYCLE
m_tps = strangeio::routine::debug::clock_time();
#endif
				
				cycle();
				/* Put the sync cycle *after* the ac cycle.
				 * The reason being that we are now currently 
				 * in the latency window. If we did it before 
				 * the ac cycle, we would be syncing at the 
				 * trigger point of sound driver's ring buffer, 
				 * and we need to get samples there ASAP... not 
				 * faff around with syncing the units
				 */
				if(m_resync) {
					
					// syncs really shouldn't happen too often
					if(m_resync_flags) {
						if(m_resync_flags & (sync_flag)sync_flags::upstream) {
							/* upstream takes priority for now
							 * because it will override the entire
							 * sync.
							 */
							sync((sync_flag)sync_flags::upstream);
							m_resync_flags ^=  (sync_flag)sync_flags::upstream;
						}
						sync((sync_flag)m_resync_flags);

						m_resync_flags = 0;
					} else {
						cycle(cycle_type::sync);
					}

					// Switch off the flag (might need to lock?)
					m_resync = false;


					
				}
				--m_cycle_queue;

#if DEBUG_PATH & PATH_CYCLE
m_tpe = strangeio::routine::debug::clock_time();
std::cout << "CycleTime: "
<< siortn::debug::clock_delta_us(m_tps, m_tpe)
<< std::endl << std::endl;
#endif
			}
sched_yield();
		}
		m_active = false;
		
	}, "rack");

	// busy loop until activation
	while(!m_active) {
		asm(""); //  don't optimise away loop!
		continue;
	}
}

void rack::stop() {
	m_running = false;
	m_trigger.notify_one();
	m_rack_thread.join();
}

bool rack::active() {
	return m_active;
}

bool rack::running() {
	return m_running;
}

void rack::assign_schpolicy(strangeio::thread::sched_desc policy) {
	m_schpolicy = policy;
}
