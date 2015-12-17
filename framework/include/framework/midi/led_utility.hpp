#ifndef LED_UTILITY_HPP
#define	LED_UTILITY_HPP

namespace strangeio {
namespace midi {

enum class led_mode {
	steady, blink, off
};

struct led_state {
	std::string device;
	led_mode mode;
	int code;
	uint8_t value;
};

class led_utility {
public:
	virtual void trigger_led(std::string unit, unsigned int state) = 0;
};

}
}

#endif	/* LED_UTILITY_HPP */

