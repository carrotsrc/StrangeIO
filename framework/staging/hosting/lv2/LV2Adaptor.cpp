/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "LV2Adaptor.h"

using namespace strangeio::Hosting;

LV2Adaptor::LV2Adaptor(std::string uname) : RackUnit(uname) {

}
void LV2Adaptor::setPlatform(const LV2Platform* platform) {
	mPlatform = platform;
}

void LV2Adaptor::loadPlugin(std::string uri, bool manual) {
	mPlugin = ((LV2Platform*) mPlatform)->getPlugin(uri);
	if(!mPlugin)
		return;

	auto inputs = mPlugin->getPortsOfType(LV2Port::Input);
	auto outputs = mPlugin->getPortsOfType(LV2Port::Output);

	if(manual)
		return;

	std::vector<const LV2Port*>::const_iterator it;
	for(it = inputs.begin(); it != inputs.end(); it++) {
		addJack((*it)->symbol, JACK_SEQ);
	}

	for(it = outputs.begin(); it != outputs.end(); it++) {
		addPlug((*it)->symbol);
	}
}

bool LV2Adaptor::initPlugin() {
	if(!mPlugin)
		return false;

	return mPlugin->init();
}

void LV2Adaptor::feedPlugin(Jack *jack) {

}

