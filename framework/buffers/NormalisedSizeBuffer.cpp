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
#include "NormalisedSizeBuffer.h"
#include <iterator>
using namespace RackoonIO::Buffers;
/** Constructor for a buffer
 * 
 * The constructor initialises the buffer to the normal period size and
 * the overflow to leave room for the variable sized input.
 *
 * @param normal The normalised number of samples to output
 * @param overflow The number of samples of extra space for variable sized input periods
 */
NormalisedSizeBuffer::NormalisedSizeBuffer ( int normal, int overflow ) {
	nSize = normal;
	buffer = new PcmSample[nSize];
	remainder = new PcmSample[nSize+overflow];

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
NormalisedSizeBuffer::State NormalisedSizeBuffer::supply (const PcmSample* period, int pSize) {
	if(rWrite < rRead && (rWrite + pSize) > rRead)
		return OVERFLOW;

	int delta = pSize;
	PcmSample *pRead = (PcmSample*) period;

	if(rWrite + pSize >= bound) {
		delta = bound - rWrite;
		std::copy(pRead, pRead+delta, rWrite);
		rWrite = remainder;
		pRead += delta;
		delta = pSize-delta;

	}

	std::copy(pRead, pRead+delta, rWrite);
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
 * @param period A PcmSample pointer to the period to flush the samples into
 *
 */
void NormalisedSizeBuffer::flush (PcmSample *period) {
	if(state == PARTIAL || load < nSize)
		return;

	int delta = nSize;
	PcmSample* bWrite = period;

	if(rRead + nSize >= bound) {
		delta = bound - rRead;
		std::copy(rRead, rRead+delta, bWrite);
		rRead = remainder;
		bWrite += delta;
		delta = nSize - delta;
	}

	std::copy(rRead, rRead+delta, bWrite);
	rRead += delta;
	load -= nSize;
	state = (load >= nSize) ? DISPATCH : PARTIAL;

	//return buffer;
}


/** Retrieve the current state of the buffer
 *
 * @return The current state
 */
NormalisedSizeBuffer::State NormalisedSizeBuffer::getState () {
	return state;
}
// test
