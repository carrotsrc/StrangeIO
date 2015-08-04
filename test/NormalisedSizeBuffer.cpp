#include "framework/buffers/NormalisedSizeBuffer.h"
#include "framework/common.h"

void dprint(const PcmSample *d) {
	std::cout << "---" << std::endl;
	for(int i = 0; i < 8; i++)
		std::cout << d[i] << " ";

	std::cout << std::endl;
}
int main( void ) {
	RackoonIO::Buffers::NormalisedSizeBuffer buffer(8, (16<<2)-1);
	PcmSample testA[22], testB[27];
	int j = 1;
	for(int i = 0; i < 22; i++)
		testA[i] = j++;

	for(int i = 0; i < 27; i++)
		testB[i] = j++;

	PcmSample dispatch[8];

	auto cState = RackoonIO::Buffers::NormalisedSizeBuffer::DISPATCH;

	buffer.supply(testA, 22);
	buffer.reset();

	while(j < 49<<3) {
		// First check
		if(buffer.getState() == RackoonIO::Buffers::NormalisedSizeBuffer::DISPATCH) {
			buffer.flush(dispatch);
			dprint(dispatch);
			continue;
		}

		// if we're here then the buffer is
		// partial
		if((cState = buffer.supply(testB, 27)) == RackoonIO::Buffers::NormalisedSizeBuffer::DISPATCH) {
			buffer.flush(dispatch);
			dprint(dispatch);
		} else if(cState == RackoonIO::Buffers::NormalisedSizeBuffer::OVERFLOW)
			std::cout << "Overflow Occurred" << std::endl;

		for(int i = 0; i < 27; i++)
			testB[i] = j++;
		
	}

	std::cout << "Done!" << std::endl;

}
