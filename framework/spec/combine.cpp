#include "framework/spec/combine.hpp"
using namespace strangeio::component;
using namespace strangeio::spec;
combine::combine(std::string model, std::string label)
	: unit(unit_type::combine, model, label)
{

}

void sync_line(sync_profile& profile, sync_flag flags, unsigned int line) {
	
}