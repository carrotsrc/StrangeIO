#include <iostream>
#include "framework/memory/BitfieldCache.h"

int main( void ) {
	std::cout << "Testing memory manager" << std::endl;
	PcmSample *blockA, *blockB;
	RackoonIO::BitfieldCache memManager;

	memManager.init(512, 150);
	memManager.__print_state();

	blockA = memManager.alloc(1);
	memManager.__print_state();
	
	blockB = memManager.alloc(1);
	memManager.__print_state();

	memManager.free(blockB);
	memManager.__print_state();

	blockB = memManager.alloc(1);
	memManager.__print_state();

	memManager.free(blockA);
	memManager.__print_state();

	std::cout << memManager._maxAlloc() << std::endl;
}
