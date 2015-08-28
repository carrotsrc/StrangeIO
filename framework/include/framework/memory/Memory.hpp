#ifndef __MEMORY_HPP_1440757499__
#define __MEMORY_HPP_1440757499__
#include "framework/fwcommon.hpp"
namespace StrangeIO {
namespace Memory {

	struct CacheHandle {
		PcmSample* ptr;
		bool in_use;
		unsigned int num_blocks;
	};
}
}

#endif