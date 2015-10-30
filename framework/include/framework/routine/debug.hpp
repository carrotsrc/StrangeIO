#ifndef DEBUG_HPP__1441832734
#define DEBUG_HPP__1441832734
#include <chrono>
namespace strangeio {
namespace routine {
namespace debug {
	using us = std::chrono::microseconds;
	using ms = std::chrono::milliseconds;
	using steady = std::chrono::steady_clock;
	using tp = steady::time_point;
	
	inline std::chrono::steady_clock::time_point clock_time(){
				return steady::now();
	}

	inline int clock_delta_us(
				std::chrono::steady_clock::time_point start,
				std::chrono::steady_clock::time_point end
				) {
		return std::chrono::duration_cast<us>(end-start).count();
	}

	inline int clock_delta_ms(
				std::chrono::steady_clock::time_point start,
				std::chrono::steady_clock::time_point end
				) {
		return std::chrono::duration_cast<ms>(end-start).count();
	}
        
        inline std::chrono::steady_clock::time_point zero_timepoint() {
            std::chrono::time_point<steady, std::chrono::duration<int>> tpt(std::chrono::duration<int>(0));
            return std::chrono::steady_clock::time_point(tpt);
        }
        
        inline auto epoch_ns() {
            return steady::now().time_since_epoch().count();
        }
        

}
}
}

#endif