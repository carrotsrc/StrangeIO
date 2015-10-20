#ifndef SHARED_SYNC_HPP
#define	SHARED_SYNC_HPP

#include <atomic>
#include <condition_variable>
#include <functional>
#include <vector>
#include <tuple>
#include <mutex>

#include "framework/thread/ntask_sync_utility.hpp"
namespace strangeio {
namespace thread {

	
class ntask_sync
	: ntask_sync_utility 
{
public:
	ntask_sync();

	virtual ~ntask_sync();
	
	ntask_sync& reset();
	void clear();

	ntask_sync& set_trigger(int value);
	
	ntask_sync& start_and_wait(std::function<void(ntask_sync_utility&)> task);
	ntask_sync& start(std::function<void(ntask_sync_utility&)> task);
	ntask_sync& then(std::function<void(ntask_sync_utility&)> task);
	ntask_sync& then_wait(std::function<void(ntask_sync_utility&)> task);

	void go();
	
	void done();
	
private:
	int m_value;
	std::atomic<int> m_shr;
	std::atomic_flag m_trigger;
	std::condition_variable m_cv;
	
	std::vector<std::tuple<std::function<void(ntask_sync_utility&)>, bool>> m_chain;
	
	ntask_sync& add_task(std::function<void(ntask_sync_utility&)> task, bool wait);
};

}
}

#endif	/* SHARED_SYNC_HPP */

