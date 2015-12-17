/* 
 * File:   led_toggle_utility.cpp
 * Author: cfg
 * 
 * Created on 17 December 2015, 16:41
 */

#include "framework/midi/led_toggle_utility.hpp"

using namespace strangeio::midi;

void led_toggle_utility::set_led_utility(led_utility* led) {
	m_led = led;
}

void led_toggle_utility::toggle_led_state(std::string unit, int state) {
	m_led->trigger_led(unit, state);
}