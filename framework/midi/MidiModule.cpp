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
#include "MidiModule.h"
#include <iomanip>

using namespace RackoonIO;

MidiModule::MidiModule(string port, string name) {
	portName = port;
	alias = name;
	inMidi = nullptr;
}

bool MidiModule::init() {
	int err = 0;
	if((err = snd_rawmidi_open(&inMidi, NULL, portName.c_str(), SND_RAWMIDI_SYNC)) < 0) {
		std::cerr << alias << ": Error opening Midi on port " << portName << ": " <<
			snd_strerror(err) << std::endl;
		return false;
	}
	return true;
}

void MidiModule::cycle() {
	while(mRunning) {
		MidiCode code = flush();

		if(code.f == 0)
			return;

		try { bindings.at(code.n)(code.v); }
		catch(const std::out_of_range& oor) {}
	}
}


MidiCode MidiModule::flush() {
	int err = 0;
	MidiCode code;
	if((err = snd_rawmidi_read(inMidi, (void*)&code, 3)) < 0) {
		if(err == -EAGAIN || err == -EBUSY)
			return {0,0,0};

		std::cerr << alias << ": Critical error Reading Midi on port " << portName << " - " <<
			snd_strerror(err) << std::endl;
	}
	return code;
}

std::string MidiModule::getAlias() {
	return alias;
}

std::string MidiModule::getPort() {
	return portName;
}

void MidiModule::addBinding(double code, std::function<void(int)> func) {
	bindings.insert(std::pair< int, std::function<void(int)> >((int)code, func));
}

void MidiModule::start() {
	mRunning = true;
	mThread = std::thread(&MidiModule::cycle, this);
}

void MidiModule::stop() {
	mRunning = false;
}
