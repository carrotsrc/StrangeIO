#ifndef COMBINE_HPP_1441632935__
#define COMBINE_HPP_1441632935__
#include <vector>

#include "framework/component/unit.hpp"

namespace strangeio {
namespace spec {

class combine : public strangeio::component::unit {
public:
	combine(std::string model, std::string label);
	virtual void feed_line(strangeio::memory::cache_ptr samples, int line) = 0;
	
	virtual void sync_line(strangeio::component::sync_profile& profile,
							strangeio::component::sync_flag flags, 
							unsigned int line);

#if DEVBUILD
	bool db_input_active(unsigned int id) { return input_active(id); };
#endif
protected:
	virtual strangeio::component::cycle_state cycle() = 0;
	virtual strangeio::component::cycle_state init() = 0;
	void init_input_states();
	bool input_active(unsigned int id);
	bool all_inactive();
	bool all_active();

private:
	std::vector<bool> m_input_state;
};

}
}

#endif
