#ifndef __UNIT_HPP_1440406068__
#define __UNIT_HPP_1440406068__

#include <memory>
#include <map>
#include <functional>

#include "framework/component/component.hpp"
#include "framework/component/linkable.hpp"
#include "framework/component/rack_utility.hpp"

#include "framework/midi/midi.hpp"
#include "framework/memory/cptr_utility.hpp"

namespace strangeio {
namespace component {

using midi_method=std::function<void(midi::msg)>;
using midi_handler_map=std::map<std::string, midi_method>;

class unit : public linkable, public memory::cptr_utility {
public:
	unit(unit_type utype, std::string umodel, std::string ulabel);
	

	void set_rack(RackUtilityInterface* rack);

	// Description and State methods
	unit_type utype() const;
	std::string umodel() const;
	std::string ulabel() const;
	component_state cstate() const;

	// Communication methods
	cycle_state cycle_line(cycle_type cycle);
	void sync_line(profile & profile, sync_flag flags = 0);
	void feed_line(memory::cache_ptr samples, int line) = 0;

	// Profiling
	const profile& unit_profile() const;
	virtual void set_configuration(std::string key, std::string value) = 0;

	// Midi
	const midi_handler_map& midi_handlers();;

protected:
	void change_cstate(component_state state);
	void register_metric(profile_metric type, int value);
	void register_metric(profile_metric type, float value);
	const profile& line_profile() const;

	void log(std::string mout);
	virtual cycle_state cycle() = 0;
	virtual cycle_state init() = 0;

	void register_midi_handler(std::string binding_name, midi_method method);

private:
	midi_handler_map m_handlers;
	const unit_type m_utype;
	const std::string m_umodel, m_ulabel;
	component_state m_cstate;

	rack_utility* m_rack;

	profile m_line_profile, m_unit_profile;

};

using unit_uptr = std::unique_ptr<unit>;
using unit_sptr = std::shared_ptr<unit>;
using unit_wptr = std::weak_ptr<unit>;

typedef unit*(*UnitBuilderPtr)(std::string);

#define UnitBuilder(unit) extern "C" StrangeIO::Component::Unit* Build##unit(std::string label){return new unit(label);}

} // Component
} // StrangeIO
#endif

