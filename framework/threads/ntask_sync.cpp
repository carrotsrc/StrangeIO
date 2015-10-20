/* 
 * File:   shared_sync.cpp
 * Author: charlie
 * 
 * Created on 16 October 2015, 10:56
 */
#include <iostream>


#include "framework/thread/ntask_sync.hpp"

using namespace strangeio::thread;
using task_tuple = std::pair<std::function<void(ntask_sync_utility&)>, bool>;

ntask_sync::ntask_sync()
	: m_value(0)
	, m_shr(0) 
{
	m_trigger.clear();
}

ntask_sync::~ntask_sync() {
	
}

ntask_sync& ntask_sync::reset() {
	m_shr = 0;
	return *this;
}

ntask_sync& ntask_sync::set_trigger(int value) {
	m_value = value;
	return *this;
}

ntask_sync& ntask_sync::start(std::function<void(ntask_sync_utility&)> task) {
	return add_task(task, false);
	return *this;
}

ntask_sync& ntask_sync::then(std::function<void(ntask_sync_utility&)> task) {
	return add_task(task, false);
}

ntask_sync& ntask_sync::start_and_wait(std::function<void(ntask_sync_utility&)> task) {
	return add_task(task, true);
}

ntask_sync& ntask_sync::then_wait(std::function<void(ntask_sync_utility&)> task) {
	return add_task(task, true);
}

void ntask_sync::go() {
	std::mutex m;
	
	for(auto& pair : m_chain) {
		auto is_waiting = std::get<1>(pair);
		std::get<0>(pair)(*this);
		
		if(is_waiting) {
			std::cout << "Waiting" << std::endl;
			std::unique_lock<std::mutex> lk(m);
			m_cv.wait(lk);
		}

	}
	reset();
}

void ntask_sync::clear() {
	m_chain.clear();
}

void ntask_sync::done() {
	m_num_handles--;
	std::cout << "Done Number of handles " << m_num_handles << std::endl;
	if(!m_num_handles) {
		std::cout << "final task" << std::endl;
		if(!m_trigger.test_and_set()) {
			std::cout << "notifying" << std::endl;
			m_cv.notify_one();
		}
	}
}

ntask_sync& ntask_sync::add_task(std::function<void(ntask_sync_utility&)> task, bool wait) {
	m_chain.push_back(std::tuple<std::function<void(ntask_sync_utility&)>, bool>(task, wait));
	return *this;
}