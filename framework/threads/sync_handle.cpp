#include <iostream>

#include "framework/thread/sync_handle.hpp"

using namespace strangeio::thread;

sync_handle::sync_handle(sync_task_utility* stu, int id)
	: m_task_utility(stu)
	, m_task_id(id)
{
	std::cout << "* Task " << m_task_id << std::endl;
}

sync_handle::sync_handle(sync_handle& orig)
	: m_task_utility(orig.m_task_utility)
	, m_task_id(orig.m_task_id)
{
	std::cout << "Copied " << m_task_id << std::endl;
	orig.m_task_utility = nullptr;
	orig.m_task_id = 0;
}
 

sync_handle::sync_handle(sync_handle&& orig)
	: m_task_utility(orig.m_task_utility)
	, m_task_id(orig.m_task_id)
{
	std::cout << "Moved " << m_task_id << std::endl;
	orig.m_task_utility = nullptr;
	orig.m_task_id = 0;
}

sync_handle::~sync_handle() {
	std::cout << "~ Task " << m_task_id << std::endl;
	if(!m_task_id) return;
	m_task_utility->done();
}

sync_handle& sync_handle::operator =(sync_handle&& orig) {
	std::cout << "= Task " << m_task_id << std::endl;
	m_task_utility = orig.m_task_utility;
	m_task_id = orig.m_task_id;
	return *this;
}

int sync_handle::get_id() {
	return m_task_id;
}

