#ifndef __RACK_HPP_1440421209__
#define __RACK_HPP_1440421209__

#include <map>
#include <atomic>

#include "framework/component/component.hpp"
#include "framework/component/unit.hpp"
#include "framework/memory/cache_utility.hpp"

namespace strangeio {
namespace component {

struct rack_profile {
	profile_duration sync_duration;
	profile_duration cycle_duration;
};

class rack : public rack_utility {
public:
	rack();
	~rack();

	// Connections
	void add_mainline(std::string name);
	bool connect_mainline(std::string mainline, std::string unit);
	bool connect_units(std::string from, std::string out, std::string to, std::string in);

	// Units
	void add_unit(unit_uptr unit);
	const std::map<std::string, unit_sptr> & get_units();
	unit_wptr get_unit(std::string label);
	void clear_units();

	// Cache
	void set_cache_utility(memory::cache_utility* cache);

	// Communication
	void trigger_sync();
	void trigger_cycle();

	void sync(sync_flag flags);
	cycle_state cycle(cycle_type type = cycle_type::ac);

	// Profile stats
	const RackProfile& rack_profile();
	bool profile_line(profile & profile, std::string mainline);

protected:
	
private:
	std::map<std::string, unit_wptr> m_mainlines;
	std::map<std::string, unit_sptr> m_mounted;
	memory::cache_utility* m_cache;
	RackProfile m_rack_profile;

	std::atomic<bool> m_resync;

	// Profile methods
	void profile_sync(sync_flag flags);
};

} // Component
} // StrangeIO
#endif

