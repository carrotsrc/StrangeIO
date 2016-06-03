#ifndef REGISTRABLE_HPP
#define REGISTRABLE_HPP
namespace strangeio {
namespace component {

enum class ctype {
		unspec,
		core,
		unit
};

struct rhandle {

	long id;
	ctype type;
	std::string model;
	std::string label;
};

/**
 * Describes an object that can be registered with the system
 */
class registrable {
public:
	/**
	 * Constructor provides the details for registration
	 * @param model The model of the component
	 * @param label The label of the component
	 * @param type The type of component
	 */
	registrable(std::string model, std::string label, ctype type);
	registrable(const registrable& orig) = delete;
	virtual ~registrable();
	
	/**
	 * Set the handle of the component
	 * 
	 * @param handle The handle of the component
	 */
	void set_handle(rhandle handle);
	
	/**
	 * Get the handle of the unit
	 * 
	 * @return handle value
	 */
	rhandle handle();
	
	/**
	 * Set the registration ID of the component
	 * 
	 * @param id The ID of the component
	 */
	void set_id(long id);
	
	/**
	 * Get the system ID of the compenent
	 * @return The numeric ID
	 */
	long sysid() const;
	
	/**
	 * Get the model of the component
	 * 
	 * @returns The string model
	 */
	std::string umodel() const;
	
	/**
	 * Get the label of the component
	 * 
	 * @return The string label 
	 */
	std::string ulabel() const;
	
	/**
	 * Get the component type of the component
	 * 
	 * @return The ctype
	 */
	ctype type() const;
protected:
	const std::string m_model, m_label;
	const ctype m_type;
	

private:
	unsigned long m_component_id;
	rhandle m_handle;
};

}
}
#endif /* REGISTRABLE_HPP */

