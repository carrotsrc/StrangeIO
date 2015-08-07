 /* Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

#include "BitfieldCache.h"
using namespace StrangeIO;

BitfieldCache::BitfieldCache() {

}

void BitfieldCache::init (int bSize, int nBlocks) {
	int sbit = numBlocks>>3;
	blockSize = bSize;
	numBlocks = nBlocks;
	freeBlocks = new unsigned char[sbit]();

	blocks = (PcmSample*) calloc(numBlocks*blockSize, sizeof(PcmSample));
	first = blocks;
	last = blocks+(blockSize*numBlocks);
	mid = first + ((numBlocks>>1)*blockSize);
}

PcmSample *BitfieldCache::alloc(int num) {
	mcache.lock();
	int byte, bit, nb, located = 0;
	nb = numBlocks>>3;

	for(byte = 0; byte < nb; byte++) {
		if(freeBlocks[byte] < 0xff) {
			for(bit = 0; bit < 8; bit++) {
				if(!((1<<bit)&freeBlocks[byte])) {
					freeBlocks[byte] |= (1<<bit);
					located = 1;
					break;
				}
			}
			break;
		}

	}
	if(!located) {
		mcache.unlock();
		std::cout << "!!! Allocator Failure" << std::endl;
		std::cout << "byte: " << byte << "\tbit: " << bit << std::endl;
		std::cout << "numAlloc: " << dbg_numAlloc << std::endl;
		std::cout << "maxAlloc: " << dbg_maxAlloc << std::endl;
		return nullptr;
	}
	dbg_numAlloc++;
	dbg_maxAlloc = (dbg_numAlloc > dbg_maxAlloc) ? dbg_numAlloc : dbg_maxAlloc;

	PcmSample *mem = blocks + (((byte<<3)+bit)*(blockSize*sizeof(PcmSample)));

	mcache.unlock();
	return mem;
}

void BitfieldCache::__print_state() {
	int byte, bit, nb;
	nb = numBlocks>>3;

	for(byte = 0; byte < nb; byte++) {
		for(bit = 0; bit < 8; bit++) {
			if(!((1<<bit)&freeBlocks[byte])) {
				std::cout << "-";
			} else {
				std::cout << "+";
			}
		}

	}
	std::cout << std::endl << std::endl;
}

void BitfieldCache::free(PcmSample *mem) {
	mcache.lock();
	int block, byte, bit;
	block = ((int)(mem - first)/sizeof(PcmSample))/blockSize;
	if(block > numBlocks) {
		mcache.unlock();
		return;
	}


	dbg_numAlloc--;

	byte = block>>3;
	bit = block-(byte<<3);
	freeBlocks[byte] ^= 1<<bit;
	mcache.unlock();
}



