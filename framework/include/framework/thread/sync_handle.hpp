/* 
 * File:   sync_handle.hpp
 * Author: charlie
 *
 * Created on 16 October 2015, 12:03
 */

#ifndef SYNC_HANDLE_HPP
#define	SYNC_HANDLE_HPP
#include <condition_variable>
#include <atomic>

#include "framework/thread/sync_task_utility.hpp"

namespace strangeio {
namespace thread {

	
class sync_handle {
public:
	sync_handle(sync_task_utility* stu, int id);
	sync_handle(sync_handle& orig);
	sync_handle(sync_handle&& orig);
	virtual ~sync_handle();
	
	sync_handle& operator=(sync_handle&& orig);
	
	int get_id();
	
private:
	sync_task_utility* m_task_utility;
	int m_task_id;
};

}
}
#endif	/* SYNC_HANDLE_HPP */

