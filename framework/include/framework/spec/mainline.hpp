#ifndef MAINLINE_HPP_1441882754__
#define MAINLINE_HPP_1441882754__
#include <vector>

#include "framework/component/unit.hpp"

namespace strangeio {
namespace spec {

class mainline : public strangeio::component::unit {
public:
	mainline(std::string model, std::string label);
	virtual void feed_line(strangeio::memory::cache_ptr samples, int line) = 0;
	
	virtual void sync_line(strangeio::component::sync_profile& profile,
							strangeio::component::sync_flag flags, 
							unsigned int line);

protected:
	virtual strangeio::component::cycle_state cycle() = 0;
	virtual strangeio::component::cycle_state init() = 0;

private:
	using strangeio::component::unit::m_upstream;
	using strangeio::component::unit::m_unit_profile;

};

}
}

#endif