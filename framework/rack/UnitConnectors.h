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
#include "framework/common.h"

namespace RackoonIO {

class RackUnit;

/** @file
 */

/** The different feed states */
enum FeedState {
	FEED_OK, ///< Data received or ready and waiting
	FEED_WAIT, ///< Cannot accept data
	FEED_BLOCKED ///< Blocked, pretty much unsused at the moment
};

/** Different states used in rack cycles */
enum RackState {
	RACK_AC, ///< The all important AC signal
	RACK_RESET, ///< Rack signals a reset
	RACK_OFF, ///< Rack signals a close down
	RACK_UNIT_OK, ///< RackUnit cycles successfully
	RACK_UNIT_FAILURE ///< Problem in RackUnit cycle
};

/** Different forms of connector
 *
 * This is closed to deprecated
 * since all units so far have just
 * used seq, apart from mainline
 * units which use AC
 */
enum ConnectorType {
	JACK_AC, ///< An AC input jack
	JACK_THREADED,
	JACK_SEQ ///< A sequential jack
};

/** The base class for a unit connector
 */
class UnitConnector {
protected:
	RackUnit *weld; ///< The unit that the connector is 'welded' to (owner)
public:
	/** The connector is instantiated with it's weld unit */
	UnitConnector(RackUnit *wunit)
	{ weld = wunit; }

	string name; ///< The name of the connector
	bool connected; ///< Whether the connector is actually connected to something

};

/** A base class jack (jack socket), the input connector on units
 *
 * These are the connectors that units feed data
 * into other units.
 *
 * A unit can have more than one jack, which effectively makes them
 * multi-channel. For instance, a mixer may have two jacks to
 * receive data from two different daisychains of units.
 *
 * @todo
 * When the samples are of floating point type, the feed/flush methods need updating
 */
class Jack : public UnitConnector {
public:
	int frames; ///< The number of sample frames (will be deprecated)

	Jack(RackUnit *wunit) : UnitConnector(wunit) {};

	/** Method called to feed data into the respective unit
	 *
	 * @param data The sample data to feed into the unit
	 * @return The state of the feed- FEED_OK if accepted; otherwise FEED_WAIT
	 */
	virtual FeedState feed(float *data) = 0;

	/* Method for flushing data that has been fed into the jack
	 *
	 * This method is used by the jack's weld unit to flush
	 * the samples from the jack. It is a pointer to a pointer
	 * so the unit receives a pointer to the original memory
	 * when the jack is flushed.
	 *
	 * @param buffer A pointer to the pointer that will hold the address of the sample data
	 * @return The state of the feed - FEED_OK if data was flushed fine; FEED_WAIT on a flush problem
	 */
	virtual FeedState flush(float **buffer) = 0;

	/** Method to propogate a block signal downstream
	 */
	void block();

	/** Rack cycle method to push signals down the connectors
	 *
	 * Usually this will be pushing AC down the daisychain,
	 * otherwise state changes
	 *
	 * @param state The current state signal being pushed down
	 * @return RACK_UNIT_OK; otherwise RACK_UNIT_ERROR on a problem
	 */
	RackState rackFeed(RackState state);
};

/** A sequential jack
 *
 * These are the most used. They act as the point of contact
 * for feeding data into another unit. They signal their weld
 * unit when they have data being fed to them and return
 * the state of the feed back to the calling unit.
 */
class SeqJack : public Jack {
protected:
	bool full; ///< A toggle when the buffer is already under load
	float *buffer; ///< The buffer for transferring the samples
public:
	//** Initialise the connector with it's weld unit */
	SeqJack(RackUnit *weld) : Jack(weld) {full = false;};
	FeedState feed(float *);
	FeedState flush(float **);

};


/** The plug (jack plug) is the connector for connecting to a jack
 *
 * Plug's weld units are those transferring the data.
 *
 * Different plugs on a unit can be plugged into different jacks.
 * This allows a unit to transfer to different units.
 *
 * The plug is essentially used to find the jack connector
 * of the next unit; beyond that, not much else.
 */
class Plug : public UnitConnector {
public:
	Plug(RackUnit *wunit) : UnitConnector(wunit) {};
	RackUnit *unit; ///< The next unit in the daisy chain
	Jack *jack; ///< The jack to feed
};

}
#endif
