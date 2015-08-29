#ifndef __RACK_HPP_1440421209__
#define __RACK_HPP_1440421209__
#include <map>
#include <atomic>

#include "framework/component/Component.hpp"
#include "framework/component/Unit.hpp"
#include "framework/memory/CacheUtilityInterface.hpp"

namespace StrangeIO {
namespace Component {

struct RackProfile {
	ProfileDuration sync_duration;
	ProfileDuration cycle_duration;
};

class Rack : public RackUtilityInterface {
public:
	Rack();
	~Rack();

	// Connections
	void add_mainline(std::string name);
	bool connect_mainline(std::string mainline, std::string unit);
	bool connect_units(std::string from, std::string out, std::string to, std::string in);

	// Units
	void add_unit(unit_uptr unit);
	const std::map<std::string, unit_sptr> & get_units();
	unit_wptr get_unit(std::string label);
	void clear_units();

	//Cache
	void set_cache_utility(Memory::CacheUtilityInterface* cache);

	// Communication
	void toggle_resync();
	void sync(SyncFlag flags);
	CycleState cycle(CycleType type = CycleType::Ac);

	// Profile stats
	const RackProfile& rack_profile();
	bool profile_line(Profile & profile, std::string mainline);

protected:
	
private:
	std::map<std::string, unit_wptr> m_mainlines;
	std::map<std::string, unit_sptr> m_mounted;
	Memory::CacheUtilityInterface* m_cache;
	RackProfile m_rack_profile;

	std::atomic<bool> m_resync;

	// Profile methods
	void profile_sync(SyncFlag flags);
};

} // Component
} // StrangeIO
#endif

