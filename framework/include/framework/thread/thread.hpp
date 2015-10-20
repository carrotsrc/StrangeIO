#ifndef __THREAD_HPP_1441205882__
#define __THREAD_HPP_1441205882__
#include <functional>
#include <vector>
namespace strangeio {
namespace thread {

using pkg_task = std::function<void(void)>;
using pkg_queue = std::vector<pkg_task>;
 
} // thread
} // strangeio
 
#endif