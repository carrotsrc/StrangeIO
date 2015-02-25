#include <iostream>
#include "framework/memory/BitfieldCache.h"

int main( void ) {
	cout << "Testing memory manager" << endl;
	short *blockA, *blockB;
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

	cout << memManager._maxAlloc() << endl;
}
