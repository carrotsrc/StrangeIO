#ifndef __LINKABLE_HPP_1440496152__
#define __LINKABLE_HPP_1440496152__
#include <vector>
#include "framework/fwcommon.hpp"

namespace StrangeIO {
namespace Component {

class Linkable; // Forward declaration

struct LinkIn {
	std::string label;
	unsigned int id;
	Linkable* unit;
};

struct LinkOut {
	std::string label;
	unsigned int id;
	LinkIn* to;
	bool connected;
};

class Linkable {
public:
	Linkable();

	bool connect(int id, LinkIn*);

	const int has_input(std::string label) const;
	const LinkIn* get_input(int id) const;

	const int has_output(std::string label) const;
	const LinkOut* get_output(int id) const;

protected:
	void add_output(std::string label);
	void add_input(std::string label);

	bool feed_out(PcmSample*, int id);
	virtual void feed_line(PcmSample*, int line) = 0;

private:
	std::vector<LinkIn> m_inputs;
	unsigned int m_num_inputs;

	std::vector<LinkOut> m_outputs;
	unsigned int m_num_outputs;
};

} // Component
} // StrangeIO
#endif

