/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld <charlie@carrotsrc.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BITFIELDCACHE_H
#define BITFIELDCACHE_H
#include "CacheHandler.h"
namespace RackoonIO {

/** The rudimentary, prototype caching system
 *
 * This class is patterned on simple page allocators
 * in basic kernel design. The whole spread of 
 * contiguous memory is split into a series of blocks
 * and each block is represented as a bit in a long 
 * array of bytes - a bitfield.
 *
 * When a cache block is allocated and in use, the
 * respective bit is switched on.
 *
 * Once a block is returned, it is switched off.
 *
 * Scanning for single free blocks is fairly fast
 * since you scan for a byte that has a valued below
 * 0xFF. Multiple continuous blocks takes a little
 * more time.
 */
class BitfieldCache : public CacheHandler
{
public:
	BitfieldCache();
	void init(int, int);
	short *alloc(int);
	void free(short*);

	void __print_state();

private:
	char *freeBlocks;
	short *blocks;
	short *first, *mid, *last;
	std::mutex mcache;
};

}

#endif
