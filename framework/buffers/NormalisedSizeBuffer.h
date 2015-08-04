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
#ifndef NORMALISEDSIZEBUFFER_H
#define NORMALISEDSIZEBUFFER_H
#include "framework/common.h"

namespace RackoonIO {

namespace Buffers {

/** Buffer for variable-in/normalised-out sized periods
 *
 * The class is built to accept a supply of periods with 
 * a variable number of samples per period, but will always 
 * flush out a normalised size period of samples. The flushed 
 * samples will be in the sequence they were supplied, over
 * multple periods; there are no dropped samples. It can be
 * used when a process is generating more samples than can
 * transferred.
 *
 * General usage:
 *
 * Continue to supply periods until the buffer state chanages
 * from PARTIAL to DISPATCH.
 *
 * Once in DISPATCH, keep flushing the buffer until its
 * state changes to PARTIAL.
 */

/** @todo
 * Once samples are set to a standard data type (floating point) then
 * this buffer can be given a pointer to a cache object to flush into
 * directly instead of two memcpys to get it into cache
 */
class NormalisedSizeBuffer {
public:
	/// The various stats the buffer will take on
	enum State {
		DISPATCH, ///< The buffer has a dispatch waiting to be flushed
		PARTIAL, ///< The buffer's load is less than the normal size; waiting for more data
		OVERFLOW ///< The write head has caught up with the read head (Shouldn't happen)
	};

	NormalisedSizeBuffer(int normal, int overflow);
	State supply(const PcmSample*, int);
	void flush(PcmSample *period);

	void reset() { state = PARTIAL; };
	State getState();

private:
	int load;
	int nSize;

	PcmSample *buffer, *remainder, *rRead, *rWrite, *bound;
	State state;
};

/*
 * Template definitions
 */


} // Buffers
} // RackoonIO

#endif
