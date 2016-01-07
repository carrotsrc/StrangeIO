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
	
class registrable {
public:
	registrable(std::string model, std::string label, ctype type);
	registrable(const registrable& orig) = delete;
	virtual ~registrable();
	
	void set_handle(rhandle handle);
	rhandle handle();
	void set_id(long id);
	long sysid() const;
	std::string umodel() const;
	std::string ulabel() const;
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

