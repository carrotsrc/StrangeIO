#ifndef TASK_NODE_HPP
#define	TASK_NODE_HPP

#include "framework/component/linkable.hpp"
#include "framework/thread/task_utility.hpp"
namespace strangeio {
namespace component {

class task_node
	: public linkable 
	, public thread::task_utility
	
{
public:
	task_node();
	virtual ~task_node();
	cycle_state cycle_line(cycle_type type);
	void feed_line(memory::cache_ptr samples, int line);
	void sync_line(sync_profile& profile, sync_flag flags, unsigned int line);

protected:
private:
};

}
}
#endif	/* TASK_NODE_HPP */

