/*
 * Copyright (C) 2015  charlie <charlie@carrotsrc.org>
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

#include "DelayBuffer.h"
using namespace RackoonIO::Buffers;
/** Initialise the buffer size
 *
 * The interal buffer is allocated with the specified size
 */
DelayBuffer::DelayBuffer(int size) {
	bSize = size;
	buffer = new PcmSample[size];
	load = 0;
}

/** Get the current sample load
 *
 * @return The number of samples in the buffer
 */
unsigned int DelayBuffer::getLoad() {
	return load;
}

/** Supply a sample period to the buffer
 *
 * Supply a period of pSize samples. The method
 * will return the state of the buffer
 *
 * @param period A pointer to the period of samples
 * @param pSize The number of samples in period
 * @return OK if frames were copied; WAIT if the buffer's load is at full capacity
 */

DelayBuffer::State DelayBuffer::supply(const PcmSample *period, int pSize) {
	if(load + pSize > bSize)
		return WAIT;

	std::copy(period, period+pSize, buffer+load);
	//memcpy(buffer+load, period, pSize * sizeof(PcmSample));
	load += pSize;
	return OK;
}

/** Flush all the samples out of the buffer
 *
 * This method will return a pointer to the samples
 * and reset the load to zero.
 *
 * @return A const pointer to the samples
 */
const PcmSample* DelayBuffer::flush() {
	load = 0;
	return buffer;
}
/** Check whether the buffer has enough room left
 *
 * @param pSize The size of the next period
 * @return OK if load is below capacity; WAIT if at full capacity
 */
DelayBuffer::State DelayBuffer::hasCapacity(int pSize) {
	if(load + pSize > bSize)
		return WAIT;

	return OK;
}
