#ifndef SCHED_DESC_HPP
#define	SCHED_DESC_HPP

namespace strangeio {
namespace thread {

#ifdef __linux__

struct sched_desc {
	int policy;
	int priority;
        int cpu_affinity;
};

#endif


// ToDo: Implement the scheduler descriptions for
// the other platforms

}
}
#endif	/* SCHEDULER_DESC_HPP */

