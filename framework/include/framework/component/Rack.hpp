#ifndef __RACK_HPP_1440421209__
#define __RACK_HPP_1440421209__
#include <map>

#include "framework/component/Component.hpp"
#include "framework/component/Unit.hpp"

namespace StrangeIO {
namespace Component {

struct RackProfile {
	ProfileDuration sync_duration;
	ProfileDuration cycle_duration;
};

class Rack {
public:
	Rack();
	~Rack();

	void add_unit(unit_uptr unit);
	const std::map<std::string, unit_sptr> & get_units();
	unit_wptr get_unit(std::string label);

	void clear_units();
protected:
	
private:
	std::map<std::string, unit_sptr> m_mounted;
	RackProfile m_rack_profile;
};

} // Component
} // StrangeIO
#endif

