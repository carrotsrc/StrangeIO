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

#ifndef MEMORY_H
#define MEMORY_H
#include "common.h"
namespace RackoonIO {
  
class Memory
{
public:
	Memory();
	void init(int, int);
	short *first, *mid, *last;
	int blockSize, numBlocks;
	short *alloc(int);

protected:
	char *freeBlocks;
	short *blocks;
};
}
#endif // MEMORY_H
