/* 
 * File:   led_toggle_utility.hpp
 * Author: cfg
 *
 * Created on 17 December 2015, 16:41
 */

#ifndef LED_TOGGLE_UTILITY_HPP
#define	LED_TOGGLE_UTILITY_HPP
#include "framework/fwcommon.hpp"
#include "framework/midi/led_utility.hpp"
namespace strangeio {
namespace midi {
	

class led_toggle_utility {
public:
	void set_led_utility(led_utility* led);
protected:
	void toggle_led_state(std::string unit, int state);
private:
	led_utility *m_led;
};

}
}
#endif	/* LED_TOGGLE_UTILITY_HPP */

