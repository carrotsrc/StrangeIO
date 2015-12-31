#ifndef SCHEDULED_HPP
#define	SCHEDULED_HPP

#include <thread>
#include <atomic>
#include <cstdint>

#ifdef __linux__
#include <sched.h>
#endif

#include "framework/thread/sched_desc.hpp"


namespace strangeio {
namespace thread {

class scheduled {
public:
	scheduled();
    scheduled(std::string sys);
	
    scheduled(sched_desc desc, std::string sys = "schedule");
    scheduled(sched_desc desc, std::function<void(void)> func, std::string sys = "schedule");
    scheduled(scheduled&& s);
    scheduled(const scheduled&) = delete;
    scheduled(scheduled&) = delete;
		
    ~scheduled();

	scheduled& operator=(scheduled&& s);
	scheduled& operator=(const scheduled&) = delete;

	sched_desc local_policy();
	sched_desc assigned_policy();
	
	void set_system(std::string sys);

	void join();
	void start(std::function<void(void)> func);
	
	bool is_active() { return m_active; };

	
private:
	std::atomic_bool m_active;
	std::thread m_handle;
	sched_desc m_desc;
	std::string m_system;
	
	void schedule();
	void assign_tid();
	
#ifdef __linux__
public:
	pid_t thread_id();
private:
	pid_t m_tid;
#endif

};

}
}
#endif	/* SCHEDULED_HPP */

