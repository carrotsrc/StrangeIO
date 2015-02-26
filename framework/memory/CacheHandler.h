/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef CACHEHANDLER_H
#define CACHEHANDLER_H
#include "common.h"

namespace RackoonIO {

/** The class from which to derive Cache allocation handlers
 *
 * When the allocation handler is intialised it preallacates
 * a large chunk of contiguous memory. Chunks of this memeory are
 * then used and reused throughout the system.
 *
 * The Cache allocators are used, primarily, by RackUnits as they
 * process periods of samples. Instead of allocating memory every
 * time they need a fresh period - they use their cacheAlloc method
 * which passes back a free chunk of preallocated memory.
 *
 * Once the samples have been processed, the unit uses its
 * cacheFree method to free up the chunk of memory for
 * reuse in future.
 *
 * Since there are probably quite a few possible routes to
 * building a good allocator, each acts as drop-in with a
 * stable CacheHandler interface.
 */

/** @todo
 * When the sample type is switched to floating-point, the
 * interface and derived classes will have to change to handle
 * dealing with a cache of floating points
 */
class CacheHandler
{
public:
	/** Initialises a debug variable
	 */
	CacheHandler();

	/** Virtual method for initialising a cache of memory chunks
	 *
	 * This is the method that will allocate the memory used in
	 * the cache. Its implementation is specific to the child
	 * allocation handler.
	 *
	 * @param bSize The number of units in a cache block
	 * @param nBlocks The number of blocks in the cache
	 */
	virtual void init(int, int) = 0;

	/** Virtual method used to allocate new cache blocks
	 *
	 * This method is called to retrieve one or more free 
	 * chunks of memory from the cache. The implementation
	 * is specific to the child allocation handler.
	 *
	 * @param num The number of blocks to allocate
	 */
	virtual short *alloc(int) = 0;

	/** Virtual method used to free a cacheBlock in use
	 *
	 * When a client is finished with using a block of
	 * memory, it uses this method to free the chunk
	 * in the cache so it can be reused later. The 
	 * implementation is specific to the child allocation
	 * handler
	 *
	 * @parma mem A pointer to the cache block
	 */
	virtual void free(short*) = 0;


	/** Debug method for retrieving the peak number of blocks in use
	 */
	int _maxAlloc() {
		return dbg_maxAlloc;
	}

protected:
	int blockSize, numBlocks;
	int dbg_numAlloc, dbg_maxAlloc;


};
}
#endif
