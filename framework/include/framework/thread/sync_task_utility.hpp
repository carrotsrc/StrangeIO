#ifndef SYNC_TASK_UTILITY_HPP
#define	SYNC_TASK_UTILITY_HPP
namespace strangeio {
namespace thread {

class sync_task_utility {
public:
	sync_task_utility();
	virtual ~sync_task_utility();
	virtual void done() = 0;
private:
};

}
}
#endif	/* SYNC_TASK_UTILITY_HPP */

