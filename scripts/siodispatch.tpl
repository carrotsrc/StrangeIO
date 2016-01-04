@HEADER@
#ifndef @DEF@_HPP
#define	@DEF@_HPP
#include "framework/alias.hpp"
#include "framework/spec/dispatch.hpp"

class @UNIT@ : public strangeio::spec::dispatch {
public:
	enum class state{ ready };
	@UNIT@(std::string label);
	virtual ~@UNIT@();

	void feed_line(siomem::cache_ptr samples, int line);

private:	
	siocom::cycle_state cycle();
	siocom::cycle_state init();
	siocom::cycle_state resync(siocom::sync_flag flags);
};

#endif	/* @DEF@_HPP */
@SOURCE@
#include "@UNIT@.hpp"
using namespace strangeio::component;
using namespace strangeio::spec;

@UNIT@::@UNIT@(std::string label)
	: dispatch("@UNIT@", label)
{ 

//	add_input("audio_in");
//	add_output("audio");
//	register_midi_handler("play", [this](siomid::msg m) {});
//	register_midi_led("ready", 0);	

}

@UNIT@::~@UNIT@() {
}

siocom::cycle_state @UNIT@::cycle() {
	return siocom::cycle_state::complete;
}

void @UNIT@::feed_line(siomem::cache_ptr samples, int line) {
}

siocom::cycle_state @UNIT@::init() {
	log("Initialised");
	return siocom::cycle_state::complete;
}

siocom::cycle_state @UNIT@::resync(siocom::sync_flag flags) {
	return siocom::cycle_state::complete;
}

UnitBuilder(@UNIT@);



