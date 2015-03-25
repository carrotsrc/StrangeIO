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
#include "UnitConnectors.h"
#include "RackUnit.h"

using namespace RackoonIO;

RackState Jack::rackFeed(RackState state) {
	weld->rackFeed(state);
}

void Jack::block() {
	weld->block(this);
}

FeedState SeqJack::feed(PcmSample *data) {
	if(full)
		return FEED_WAIT;

	buffer = data;
	full = true;
	if(weld->feed(this) == FEED_WAIT) {
		full = false;
		return FEED_WAIT;
	}

	return FEED_OK;
};

FeedState SeqJack::flush(PcmSample **out) {
	if(!full)
		return FEED_WAIT;

	*out = buffer;
	full = false;
	return FEED_OK;
};
