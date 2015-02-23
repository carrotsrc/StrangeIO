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
#ifndef UNITCONNECTORS_H
#define UNITCONNECTORS_H
#include "common.h"

namespace RackoonIO {

class RackUnit;
enum FeedState {
	FEED_OK,
	FEED_WAIT,
	FEED_BLOCKED
};

enum RackState {
	RACK_AC,
	RACK_RESET,
	RACK_OFF,
	RACK_UNIT_OK,
	RACK_UNIT_FAILURE
};

enum ConnectorType {
	JACK_AC,
	JACK_THREADED,
	JACK_SEQ
};

class UnitConnector {
protected:
	RackUnit *weld;
public:
	UnitConnector(RackUnit *wunit)
	{ weld = wunit; }

	string name;
	bool connected;

};

class Jack : public UnitConnector {
public:
	int frames;

	Jack(RackUnit *wunit) : UnitConnector(wunit) {};
	virtual FeedState feed(short*) = 0;
	virtual FeedState flush(short**) = 0;
	void block();
	RackState rackFeed(RackState state);
};

class SeqJack : public Jack {
protected:
	bool full;
	short *buffer;
public:
	SeqJack(RackUnit *weld) : Jack(weld) {full = false;};
	FeedState feed(short *);
	FeedState flush(short **);

};


class Plug : public UnitConnector {
public:
	Plug(RackUnit *wunit) : UnitConnector(wunit) {};
	RackUnit *unit;
	Jack *jack;
};

}
#endif
