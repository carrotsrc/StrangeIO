#ifndef __UNIT_HPP_1440406068__
#define __UNIT_HPP_1440406068__

#include "framework/component/Component.hpp"

namespace StrangeIO {
namespace Component {

class Rack; // Forward declaration

class Unit {
public:
	Unit(UnitType utype, std::string umodel, std::string ulabel);
	
	// Description and State methods
	UnitType utype() const;
	std::string umodel() const;
	std::string ulabel() const;
	ComponentState cstate() const;


	// System methods
	void set_rack(Rack* ptr_rack);
	CycleState run_cycle(CycleType cycle);

protected:
	void change_cstate(ComponentState state);
	void register_metric(ProfileMetric type, int value);
	LineProfile& profile();

	CycleState cycle();
	CycleState init();

private:
	const UnitType m_utype;
	const std::string m_umodel, m_ulabel;
	ComponentState m_cstate;
	Rack* m_rack;
	LineProfile m_profile;

	void sync_line(LineProfile & profile);
};

} // Component
} // StrangeIO
#endif

