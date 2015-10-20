/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld
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

#ifndef DELAYBUFFER_HPP
#define DELAYBUFFER_HPP
#include <memory>

#include "framework/fwcommon.hpp"
#include "framework/buffer/buffer.hpp"
#include "framework/memory/cptr_utility.hpp"

namespace strangeio {
namespace buffer {

/** Buffer to act as a general delay line
 *
 * This buffer implements a general purpose delay line. It's
 * main usage is passing it periods of samples until its load
 * reaches it's capacity. When the buffer is flushed, it flushes
 * all the samples at once.
 *
 * General usage:
 *
 * Keep supplying it periods of samples until its state changes
 * from OK to WAIT, at which point is at capacity
 *
 * When you need the samples, flush it to get a pointer to all the samples
 */

/** @todo
 * Once samples are set to a standard data type (floating point) then
 * this buffer can be given a pointer to a cache object to flush into
 * directly instead of two memcpys to get it into cache
 */

class delay : public memory::cptr_utility {
public:
	/// The various states the buffer will take on
	enum state {
		ok, ///< The buffer can be supplied more data
		wait ///< The buffer is full and can't take any more
	};

	delay(int num_blocks);
	~delay();

	state supply(memory::cache_ptr samples);
	const PcmSample* flush_raw();
	unsigned int load();
	state has_capacity(int pSize);

private:
	unsigned int m_size, m_load, m_blocks;
	PcmSample *m_buffer; 
};

using delay_uptr = std::unique_ptr<strangeio::buffer::delay>;

} // Buffers
} // StrangeIO
#endif // DELAYBUFFER_H
