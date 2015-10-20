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

}
}
}

#endif