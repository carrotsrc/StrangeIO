#ifndef __UNIT_HPP_1440406068__
#define __UNIT_HPP_1440406068__

#include "framework/component/Component.hpp"
#include "framework/component/Linkable.hpp"

namespace StrangeIO {
namespace Component {


class Unit : public Linkable {
public:
	Unit(UnitType utype, std::string umodel, std::string ulabel);
	
	// Description and State methods
	UnitType utype() const;
	std::string umodel() const;
	std::string ulabel() const;
	ComponentState cstate() const;

	// Communication methods
	CycleState cycle_line(CycleType cycle);
	void sync_line(Profile & profile);
	void feed_line(PcmSample *samples, int line) = 0;

	const Profile& unit_profile() const;

protected:
	void change_cstate(ComponentState state);
	void register_metric(ProfileMetric type, int value);
	const Profile& line_profile() const;

	virtual CycleState cycle() = 0;
	virtual CycleState init() = 0;

private:
	const UnitType m_utype;
	const std::string m_umodel, m_ulabel;
	ComponentState m_cstate;

	Profile m_line_profile, m_unit_profile;

};

} // Component
} // StrangeIO
#endif

