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
#ifndef MIDIMODULE_H
#define MIDIMODULE_H
#include "common.h"
namespace RackoonIO {

typedef struct {
	char f;
	char n;
	char v;
} MidiCode;

class MidiModule {
	snd_rawmidi_t *inMidi;
	std::string portName;
	std::string alias;
	char buffer[3];

	std::map<int, std::function<void(int)> > bindings;
public:
	MidiModule(string, string);
	bool init();
	void cycle();
	MidiCode flush();

	std::string getPort();
	std::string getAlias();
	void addBinding(double, std::function<void(int)>);
};

}
#endif
