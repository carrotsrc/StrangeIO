#include "framework/component/unit.hpp"
using namespace strangeio;
class Phi : public component::unit {
public:
	Phi(std::string label);
	
	void set_configuration(std::string key, std::string value);

	void feed_line(memory::cache_ptr samples, int id);

	component::cycle_state cycle();

	component::cycle_state init();

	// Checks
	int init_count() const;
	int feed_count() const;

private:
	int m_init_count, m_feed_count;

};