#include "catch.hpp"
#include "framework/alias.hpp"
#include "framework/memory/cache_manager.hpp"
#include "framework/memory/cache_ptr.hpp"
#include "framework/buffer/circular.hpp"

#include "framework/routine/sound.hpp"

TEST_CASE( "Test Circular buffer", "[strangeio::buffer],[strangeio::buffer::circular]" ) {
	siomem::cache_manager cache(6);
	siobuf::circular cb;
	PcmSample samples_1[48], samples_2[16];
	
	cache.build_cache(32);	
	siomem::cache_ptr cptr(cache.alloc_raw(1), 1, &cache);
	
	for(int i = 0u; i < 48; i++) {
		samples_1[i] = i;
	}
	
	
	
	cb.set_size(48);
	cb.set_overwrite(true);
	cb.insert(samples_1, 48);
	
	cb.get(cptr);
	
	
	REQUIRE(cptr[0] == 0);
	REQUIRE(cptr[31] == 31);
	
	cb.get(cptr);
	
	REQUIRE(cptr[0] == 32);
	REQUIRE(cptr[31] == 15);
	

}