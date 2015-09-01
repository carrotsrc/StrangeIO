#ifndef __MEMORY_HPP_1440757499__
#define __MEMORY_HPP_1440757499__
#include "framework/fwcommon.hpp"
namespace strangeio {
namespace memory {

	struct cache_handle {
		PcmSample* ptr;
		bool in_use;
		unsigned int num_blocks;
	};
}
}

#endif