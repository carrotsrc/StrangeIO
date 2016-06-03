#ifndef MOUNT_HPP
#define	MOUNT_HPP

#include <vector>
#include "framework/component/unit.hpp"
#include "framework/component/rack_utility.hpp"

namespace strangeio {
namespace component {

/**
 * Provides the functionality for mounting and linking units in the rack
 * and maintaining the mount
 */	
class mount 
: public rack_utility{

public:	
	mount();
	mount(const mount& orig);
	virtual ~mount();

	// Connections
	/**
	 * Add a mainline socket
	 * @param name The name of the mainline socket
	 */
	void add_mainline(std::string name);
	
	/**
	 * Connect the mounted unit to the mainline socket
	 * 
	 * @param mainline Name of mainline socket
	 * @param unit Name of the mounted unit
	 * @return true of connected; otherwise false
	 */
	bool connect_mainline(std::string mainline, std::string unit);
	
	/**
	 * Connect two units together
	 * @param from The label of the mounted source unit
	 * @param out The label of the output
	 * @param to The label of the mounted sink unit
	 * @param in The label of the input
	 * @return true on connection; otherwise false
	 */
	bool connect_units(std::string from, std::string out, std::string to, std::string in);

	
	// Units
	
	/**
	 * Mount a unit
	 * 
	 * @param unit A unique_ptr to the unit
	 */
	void add_unit(unit_uptr unit);
	
	/**
	 * Get a reference to the map of mounted units
	 * 
	 * The map is keyed by the lable of the unit and a u_ptr
	 * of the unit object
	 *  
	 * @return A const reference to the internal map 
	 */
	const std::map<std::string, unit_sptr> & get_units();
	
	/**
	 * Get a weak pointer to unit with specified label
	 * @param label The label of the unit
	 * @return weak_ptr to unit object
	 */
	unit_wptr get_unit(std::string label);
	
	/**
	 * Check if a unit with specified label is mounted
	 * 
	 * @param label The label of the unit
	 * @return true if mounted; otherwise false
	 */
	bool has_unit(std::string label);
	
	/**
	 * Clear the map of mounted units
	 */
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

