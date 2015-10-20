#include "framework/thread/ntask_sync_utility.hpp"
#include <iostream>
using namespace strangeio::thread;
		
ntask_sync_utility::ntask_sync_utility() {
}

ntask_sync_utility::ntask_sync_utility(const ntask_sync_utility& orig) {
}

ntask_sync_utility::~ntask_sync_utility() {
}

sync_handle ntask_sync_utility::make_handle() {
	m_num_handles++;
	std::cout << "Handle: " << m_num_handles << std::endl;
	return std::move(sync_handle(this, m_num_handles));
}