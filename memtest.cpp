#include <iostream>
#include "framework/memory/Memory.h"

int main( void ) {
	cout << "Testing memory manager" << endl;
	RackoonIO::Memory memManager;

	memManager.init(512, 150);
	memManager.alloc(1);
}
