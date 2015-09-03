#include "framework/component/unit.hpp"
using namespace strangeio;
class TauUnit : public component::unit {
public:
	TauUnit(std::string label);
	
	void set_configuration(std::string, std::string);

	void feed_line(memory::cache_ptr samples, int id);

	component::cycle_state cycle();
	component::cycle_state init();

	// Checks
	int init_count() const;
	int feed_count() const;
	int block_count() const;
	int config_check() const;

private:
	int m_init_count, m_feed_count, m_block_count, m_config;
	const PcmSample* m_ptr;

};