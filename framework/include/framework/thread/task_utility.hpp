#ifndef __TASK_UTILITY_HPP_1441206638__
#define __TASK_UTILITY_HPP_1441206638__
#include "framework/thread/thread.hpp"
#include "framework/thread/queue_utility.hpp"

namespace strangeio {
namespace thread {

class task_utility
{
public:
	task_utility();
	void set_queue_utility(queue_utility* queue);

protected:
	void add_task(pkg_task task);

private:
	queue_utility* m_queue;
};

} // thread
} // strangeio

#endif // __TASK_UTILITY_HPP_1441206638__
