#include "framework/thread/task_utility.hpp"

using namespace strangeio::thread;

task_utility::task_utility()
	: m_queue(nullptr)
{
	
}

void task_utility::add_task(pkg_task task) {
	if(m_queue == nullptr) return;
	m_queue->add_package(task);
}

void task_utility::set_queue_utility(queue_utility* queue) {
	m_queue = queue;
}


