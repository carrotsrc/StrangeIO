#ifndef __RACK_HPP_1440421209__
#define __RACK_HPP_1440421209__
#include <map>
#include <atomic>

#include "framework/component/Component.hpp"
#include "framework/component/Unit.hpp"

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
	bool connect(std::string mainline, std::string unit);

	void add_unit(unit_uptr unit);
	const std::map<std::string, unit_sptr> & get_units();
	unit_wptr get_unit(std::string label);
	void clear_units();

	// Communication
	void toggle_resync();
	void sync(Profile & profile, SyncFlag flags);
	void cycle(CycleType type = CycleType::Ac);

protected:
	
private:
	std::map<std::string, unit_wptr> m_mainlines;
	std::map<std::string, unit_sptr> m_mounted;
	RackProfile m_rack_profile;

	std::atomic<bool> m_resync;
};

} // Component
} // StrangeIO
#endif

