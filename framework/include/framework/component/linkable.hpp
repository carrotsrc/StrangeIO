#ifndef __LINKABLE_HPP_1440496152__
#define __LINKABLE_HPP_1440496152__
#include <vector>

#include "framework/component/component.hpp"
#include "framework/component/registrable.hpp"

#include "framework/fwcommon.hpp"
#include "framework/memory/cache_ptr.hpp"


namespace strangeio {
namespace component {

class linkable; // Forward declaration

/**
 * The link going into a unit
 */
struct LinkIn {
	std::string label;
	unsigned int id;
	linkable* unit;
	bool connected;
};

/**
 * The link coming of a unit
 */
struct LinkOut {
	std::string label;
	unsigned int id;
	LinkIn* to;
	bool connected;
};

/**
 * Describes an object that can be linked together with other
 * linkable objects and provides the functionality to do the
 * linking
 */
class linkable : public registrable {
public:
	/**
	 * @param model The model of the linkable
	 * @param label The unique label of the linkable
	 * @param type The component type
	 */
	linkable(std::string model, std::string label, ctype type);

	/**
	 * Make a connection between specified output and input link
	 * @param id The id of the output
	 * @param input Pointer to the link input
	 * @return True if successfully linked; otherwise false
	 */
	bool connect(int id, LinkIn* input);
	
	/**
	 * Disconnect output
	 * 
	 * @param id The ID of the output to disconnect
	 */
	void disconnect(int id);

	/**
	 * get the id of input with specified label
	 * 
	 * @param label The string label of the input
	 * @return id >= 0 if input found; otherwise -1
	 */
	int has_input(std::string label) const;
	
	/**
	 * Get pointer to input with id
	 * 
	 * @param id The ID of the input
	 * @return pointer to LinkIn; otherwise nullptr if invalid ID
	 */
	const LinkIn* get_input(int id) const;

	/**
	 * get the id of output with specified label
	 * 
	 * @param label The string label of the output
	 * @return id >= 0 if output found; otherwise -1
	 */
	int has_output(std::string label) const;
	
	/**
	 * Get pointer to output with id
	 * 
	 * @param id The ID of the output
	 * @return pointer to LinkOut; otherwise nullptr if invalid ID
	 */
	const LinkOut* get_output(int id) const;
	
	virtual void sync_line(sync_profile& profile, sync_flag flags, unsigned int line) = 0;
	virtual void fill_line(memory::cache_ptr samples, int id) = 0;
	virtual cycle_state cycle_line(cycle_type type) = 0;

protected:
	
	/**
	 * Add an output to linkable
	 * 
	 * @param label The name of the output
	 */
	void add_output(std::string label);
	
	/**
	 * Add an input to the linkable
	 * 
	 * @param label The name of the input
	 */
	void add_input(std::string label);

	/**
	 * Check if input is connected to anything
	 * 
	 * @param id The ID of the input
	 * @return true if connected; otherwise false
	 */
	bool input_connected(unsigned int id) const;

	/**
	 * Check if output is connected to anything
	 * 
	 * @param id The ID of the output
	 * @return true if connected; otherwise false
	 */
	bool output_conncted(unsigned int id) const;

	/**
	 * Feed samples through output
	 * 
	 * @param samples The block of samples
	 * @param id The id of the output
	 * @return true if sent out; otherwise false
	 */
	bool feed_out(memory::cache_ptr samples, int id);
	
	/**
	 * Send samples through a fill line
	 * 
	 * @param samples The block of samples to send out
	 * @param id The ID of the fill-line
	 * @return true on sent out; otherwise false
	 */
	bool fill_out(memory::cache_ptr samples, int id);
	virtual void feed_line(memory::cache_ptr samples, int line) = 0;

	const std::vector<LinkIn> & inputs() const;
	const std::vector<LinkOut> & outputs() const;

private:
	std::vector<LinkIn> m_inputs;
	unsigned int m_num_inputs;

	std::vector<LinkOut> m_outputs;
	unsigned int m_num_outputs;
};

} // Component
} // StrangeIO
#endif

