#ifndef MOUNT_HPP
#define	MOUNT_HPP

#include <vector>
#include "framework/component/unit.hpp"
#include "framework/component/rack_utility.hpp"

namespace strangeio {
namespace component {
	
class mount 
: public rack_utility{

public:	
	mount();
	mount(const mount& orig);
	virtual ~mount();

	// Connections
	void add_mainline(std::string name);
	bool connect_mainline(std::string mainline, std::string unit);
	bool connect_units(std::string from, std::string out, std::string to, std::string in);

	
	// Units
	void add_unit(unit_uptr unit);
	const std::map<std::string, unit_sptr> & get_units();
	unit_wptr get_unit(std::string label);
	bool has_unit(std::string label);
	void clear_units();

protected:
	std::map<std::string, unit*> m_mainlines;
	std::map<std::string, unit_sptr> m_mounted;
	std::vector<unit*> m_raw_mainlines;
	
	virtual void mount_dependencies(unit*) = 0;

private:
};

} // Component
} // StrangeIO
#endif	/* MOUNT_HPP */

