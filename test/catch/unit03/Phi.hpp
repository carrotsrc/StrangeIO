#include "framework/component/unit.hpp"
using namespace strangeio;
class Phi : public component::unit {
public:
	Phi(std::string label);
	
	void set_configuration(std::string key, std::string value) override;

	void feed_line(memory::cache_ptr samples, int id) override;

	component::cycle_state cycle() override;

	component::cycle_state init() override;

	// Checks
	int init_count() const;
	int feed_count() const;

private:
	int m_init_count, m_feed_count;

};