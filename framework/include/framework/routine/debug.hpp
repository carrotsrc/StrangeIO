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
	
	class scoped_timer {
	public:
		scoped_timer(std::string message) 
			: m_s(clock_time())
			, m_out(nullptr)
			, m_msg(message)
		{ }

		scoped_timer(int* out) 
			: m_s(clock_time())
			, m_out(out)
			, m_msg()
		{ }

		~scoped_timer() {
			auto d = clock_delta_us(m_s, clock_time());
			if(m_out) {
				*m_out = d;
			} else	{
				std::cout 
					<< "( Profile " << d << "us ) " 
					<< m_msg << std::endl;
			}
		}

	private:
		tp m_s;
		int* m_out;
		std::string m_msg;
	};


}
}
}

#endif