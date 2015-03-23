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
template<typename T>
class NormalisedSizeBuffer {
public:
	/// The various stats the buffer will take on
	enum State {
		DISPATCH, ///< The buffer has a dispatch waiting to be flushed
		PARTIAL, ///< The buffer's load is less than the normal size; waiting for more data
		OVERFLOW ///< The write head has caught up with the read head (Shouldn't happen)
	};

	NormalisedSizeBuffer(int, int);
	State supply(const T*, int);
	const T *flush();

	void reset() { state = PARTIAL; };
	State getState();

private:
	int load;
	int nSize;

	T *buffer, *remainder, *rRead, *rWrite, *bound;
	State state;
};

/*
 * Template definitions
 */

/** Constructor for a buffer
 * 
 * The constructor initialises the buffer to the normal period size and
 * the overflow to leave room for the variable sized input.
 *
 * @param normal The normalised number of samples to output
 * @param overflow The number of samples of extra space for variable sized input periods
 */
template<typename T>
NormalisedSizeBuffer<T>::NormalisedSizeBuffer ( int normal, int overflow ) {
	nSize = normal;
	buffer = (T*) malloc(sizeof(T)*nSize);
	remainder = (T*) malloc(sizeof(T)*nSize+overflow);
	bound = remainder + (nSize+overflow);
	rWrite = rRead = remainder;
	load = 0;
	state = PARTIAL;
}

/** Supply a variable size period to the buffer
 *
 * The period is pSize number of samples located
 * at location *period. The method returns the state
 * of the buffer after supply unless OVERFLOW,
 * which means that the supply overflow the read pointer;
 * OVERFLOW means the period has not been copied.
 *
 * @param period A pointer to a period
 * @param pSize The number of samples/frames in the period
 * @return DISPATCH if the final load is greater than a normal period; PARTIAL if less
 */
template<typename T>
typename NormalisedSizeBuffer<T>::State
NormalisedSizeBuffer<T>::supply (const T* period, int pSize) {
	if(rWrite < rRead && (rWrite + pSize) > rRead)
		return OVERFLOW;

	int delta = pSize;
	T *pRead = (T*) period;

	if(rWrite + pSize >= bound) {
		delta = bound - rWrite;
		memcpy(rWrite, pRead, delta * sizeof(T)); 
		rWrite = remainder;
		pRead += delta;
		delta = pSize-delta;

	}

	memcpy(rWrite, pRead, delta*sizeof(T));
	rWrite += delta;

	load += pSize;

	state = (load >= nSize) ? DISPATCH : PARTIAL;
	return state;
}
/** Flush out a normal sized period of samples
 *
 * If there are samples waiting to be dispatched then
 * calling this method will return a pointer to the 
 * samples
 *
 * @return A pointer to the samples being flushed from the buffer
 */
template<typename T>
const T *NormalisedSizeBuffer<T>::flush () {
	if(state == PARTIAL || load < nSize)
		return nullptr;

	int delta = nSize;
	T* bWrite = buffer;

	if(rRead + nSize >= bound) {
		delta = bound - rRead;
		memcpy(bWrite, rRead, delta * sizeof(T));
		rRead = remainder;
		bWrite += delta;
		delta = nSize - delta;
	}

	memcpy(bWrite, rRead, delta * sizeof(T));
	rRead += delta;
	load -= nSize;
	state = (load >= nSize) ? DISPATCH : PARTIAL;

	return buffer;
}


/** Retrieve the current state of the buffer
 *
 * @return The current state
 */
template<typename T>
typename NormalisedSizeBuffer<T>::State
NormalisedSizeBuffer<T>::getState () {
	return state;
}


} // Buffers
} // RackoonIO

#endif
