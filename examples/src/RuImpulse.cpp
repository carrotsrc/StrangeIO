/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "RuImpulse.h"
using namespace RackoonIO;

RuImpulse::RuImpulse()
: RackUnit(std::string("RuImpulse")) {
	addJack("audio", JACK_SEQ);
	workState = IDLE;
}

FeedState RuImpulse::feed(Jack *jack) {
	Jack *j = getJack("audio");
	return FEED_OK;
}

void RuImpulse::setConfig(string config, string value) {
}

RackState RuImpulse::init() {
	workState = READY;
	return RACK_UNIT_OK;
}

RackState RuImpulse::cycle() {
	
	return RACK_UNIT_OK;
}

void RuImpulse::block() {
	
}