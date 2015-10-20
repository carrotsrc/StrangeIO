#ifndef NTASK_SYNC_UTILITY_HPP
#define	NTASK_SYNC_UTILITY_HPP
#include <atomic>

#include "framework/thread/sync_handle.hpp"

namespace strangeio {
namespace thread {
	
class ntask_sync_utility
	: sync_task_utility
{
public:
	ntask_sync_utility();
	ntask_sync_utility(const ntask_sync_utility& orig);
	virtual ~ntask_sync_utility();
	
	sync_handle make_handle();
	virtual void done() = 0;

protected:
	std::atomic<int> m_num_handles;
private:

};

} // thread
} // strangeio
#endif	/* NTASK_SYNC_UTILITY_HPP */

