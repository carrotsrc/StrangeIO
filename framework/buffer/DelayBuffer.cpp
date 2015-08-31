/*
 * Copyright (C) 2015  charlie fyvie-gauld <charlie@carrotsrc.org>
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

#include "framework/buffer/DelayBuffer.hpp"
using namespace StrangeIO::Buffer;
/** Initialise the buffer size
 *
 * The interal buffer is allocated with the specified size
 */
DelayBuffer::DelayBuffer(int size) :
m_size(size), m_load(0) {
	m_buffer = new PcmSample[size];
}

DelayBuffer::~DelayBuffer() {
	delete[] m_buffer;
}

/** Get the current sample load
 *
 * @return The number of samples in the buffer
 */
unsigned int DelayBuffer::load() {
	return m_load;
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

DelayBuffer::State DelayBuffer::supply(const PcmSample *samples, int num_samples) {
	if(m_load + num_samples > m_size)
		return WAIT;

	std::copy(samples, samples+num_samples, m_buffer+m_load);
	m_load += num_samples;
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
	m_load = 0;
	return m_buffer;
}
/** Check whether the buffer has enough room left
 *
 * @param pSize The size of the next period
 * @return OK if load is below capacity; WAIT if at full capacity
 */
DelayBuffer::State DelayBuffer::has_capacity(int num_samples) {
	if(m_load + num_samples > m_size)
		return WAIT;

	return OK;
}
