#include <iostream>
#include <string.h>

#include "framework/thread/scheduled.hpp"

#ifdef __linux__
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#endif

using namespace strangeio::thread;

scheduled::scheduled()
	: m_active(false)
	, m_system("scheduled")
	, m_tid(-1)
{ }

scheduled::scheduled(std::string sys)
	: m_active(false)
	, m_desc({0})
	, m_system(sys)
	, m_tid(-1)
{ }
scheduled::scheduled(sched_desc desc, std::string sys) 
	: m_active(false)
	, m_system(sys)
	, m_tid(-1)
{
	m_desc = desc;
}

scheduled::scheduled(sched_desc desc, std::function<void(void)> func, std::string sys)
	: m_active(false)
	, m_system(sys)
	, m_tid(-1)
	
{
	m_desc = desc;
	start(func);
}

scheduled::scheduled(scheduled&& s) {
	m_handle.swap(s.m_handle);
	m_desc = s.m_desc;
	m_active = s.m_active.load();
	m_system = s.m_system;
}

scheduled::~scheduled() { 
	if(m_active) {
		join();	
	}
};
	
scheduled& scheduled::operator=(scheduled&& s) {
	m_handle.swap(s.m_handle);
	m_desc = s.m_desc;
	m_active = s.m_active.load();
	m_system = s.m_system;
	return *this;
}

sched_desc scheduled::local_policy() {
	return m_desc;
}

void scheduled::set_system(std::string sys) {
	m_system = sys;
}


sched_desc scheduled::assigned_policy() {
	sched_desc d{-1};
	
#ifdef __linux__
	struct sched_param param;
	
	if(sched_getparam(m_tid, &param) < 0) {
		std::cout << "Errno: " << strerror(errno) << " - " << m_tid << std::endl;
		switch(errno) {
			case EINVAL: d.priority = 0; break;
			default: return d;
		}
	}
	
	auto policy = sched_getscheduler(m_tid);

	if(policy < 0)
		return d;
	
	d.policy = policy;
	d.priority = param.__sched_priority;
	
#endif
	
	return d;
}

void scheduled::join() {
	m_handle.join();
}

void scheduled::start(std::function<void(void)> func) {
	if(m_active) return;
	
	m_handle = std::thread([this, func]() {

		assign_tid();
		schedule();
		m_active = true;
		func();
		m_active = false;

	});
}

void scheduled::assign_tid() {
#ifdef __linux__
	m_tid = syscall(SYS_gettid);
#endif // TODO: The other platform specifics
}


void scheduled::schedule() {
#ifdef __linux__
	struct sched_param sparam;
	cpu_set_t cpu_aff;
	
	if(m_desc.cpu_affinity >= 0) {
		CPU_ZERO(&cpu_aff);
		CPU_SET(m_desc.cpu_affinity, &cpu_aff);
		if(sched_setaffinity(m_tid, CPU_COUNT(&cpu_aff), &cpu_aff) < 0) {
			std::cerr << "StrangeIO : # Failed to set CPU affinity: " << 
			strerror(errno) << std::endl;
		} else {
			std::cout << "StrangeIO ["<< m_system << "]: Affinity "
			<< m_desc.cpu_affinity << std::endl;
		}
	}
	
	sparam.__sched_priority = m_desc.priority;
	
	if(sched_setscheduler(m_tid, m_desc.policy, &sparam) != 0) {
		std::cerr << "StrangeIO : # Failed to set policy: " << 
		strerror(errno) << std::endl;
		
		std::cerr << m_desc.policy << "/" << m_desc.priority << std::endl;
		return;
	}
	
	auto sp = assigned_policy();
	
	std::cout << "StrangeIO ["<< m_system << "]: Set schedule policy: "
	<< sp.policy << "/" << sp.priority << std::endl;
	
#endif
}


#ifdef __linux__
pid_t scheduled::thread_id() {
	return m_tid;
}
#endif

