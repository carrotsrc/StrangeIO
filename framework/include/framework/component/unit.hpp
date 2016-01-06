#ifndef __UNIT_HPP_1440406068__
#define __UNIT_HPP_1440406068__

#include <memory>
#include <map>
#include <atomic>
#include <functional>

#include "framework/component/component.hpp"
#include "framework/component/linkable.hpp"

#include "framework/midi/midi.hpp"

#include "framework/component/rack_utility.hpp"
#include "framework/memory/cptr_utility.hpp"
#include "framework/thread/task_utility.hpp"
#include "framework/event/event_utility.hpp"
#include "framework/midi/led_toggle_utility.hpp"

namespace strangeio {
namespace component {

using midi_method=std::function<void(midi::msg)>;
using midi_handler_map=std::map<std::string, midi_method>;
using midi_led_map=std::map<std::string, int>;

class unit 
	: public linkable
	, public memory::cptr_utility
	, public thread::task_utility 
	, public event::event_utility 
	, public midi::led_toggle_utility {
public:
	unit(unit_type utype, std::string model, std::string label);

	void set_rack(rack_utility* rack);

	// Description and State methods
	unit_type utype() const;
	component_state cstate() const;

	// Communication methods
	cycle_state cycle_line(cycle_type cycle);
	virtual void sync_line(sync_profile& profile, sync_flag flags, unsigned int line);
	virtual void fill_line(memory::cache_ptr samples, int id);
	virtual void feed_line(strangeio::memory::cache_ptr samples, int line) = 0;

	// Profiling
	const sync_profile& unit_profile() const;
	const sync_profile& global_profile() const;

	// config
	virtual void set_configuration(std::string key, std::string value);

#if DEVBUILD
	virtual std::string get_configuration(std::string key);
	bool upstream_toggle() { return m_upstream; };
#endif

	// Midi
	const midi_handler_map& midi_handlers();
	const midi_led_map& midi_leds();
	bool controllable();

protected:
	sync_profile m_unit_profile;
	bool m_upstream;
	// ----

	void change_cstate(component_state state);
	void register_metric(profile_metric type, int value);
	void register_metric(profile_metric type, float value);
	const sync_profile& line_profile() const;

	void log(std::string mout);
	void trigger_cycle();
	void trigger_sync(sync_flag flags);

	virtual strangeio::component::cycle_state cycle() = 0;
	virtual strangeio::component::cycle_state init() = 0;
	virtual strangeio::component::cycle_state resync(strangeio::component::sync_flag flags);

	void register_midi_handler(std::string binding_name, midi_method method);
	void register_midi_led(std::string state_name, int state);

	inline void continue_sync(sync_profile & profile, sync_flag flags);

	void toggle_led(int state);
	
	#if CACHE_TRACKING
		virtual strangeio::memory::cache_ptr cache_alloc(unsigned int num);
	#endif
private:
	using linkable::m_model;
	using linkable::m_label;
	using linkable::m_type;

	midi_handler_map m_handlers;
	midi_led_map m_leds;
	const unit_type m_utype;
	component_state m_cstate;
	rack_utility* m_rack;
	sync_profile m_line_profile, m_global_profile;
	// ----
        
        inline void apply_unit_profile(sync_profile & profile, sync_flag flags);

};

using unit_uptr = std::unique_ptr<unit>;
using unit_sptr = std::shared_ptr<unit>;
using unit_wptr = std::weak_ptr<unit>;

typedef unit*(*UnitBuilderPtr)(std::string);

#define UnitBuilder(unit_class) extern "C" strangeio::component::unit* Build##unit_class(std::string label){return new unit_class(label);}

} // Component
} // StrangeIO
#endif

