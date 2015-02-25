#include "framework/buffers/NormalisedSizeBuffer.h"
#include "common.h"

void dprint(int *d) {
	for(int i = 0; i < 16; i++)
		cout << d[i] << " ";

	cout << endl;
}
int main( void ) {
	RackoonIO::NormalisedSizeBuffer<int> buffer(16, 16<<2);
	int testA[22], testB[27];
	for(int i = 0; i < 22; i++)
		testA[i] = i+1;

	for(int i = 0; i < 27; i++)
		testB[i] = i+23;

	int *dispatch;

	RackoonIO::NormalisedSizeBufferState cState = RackoonIO::NormalisedSizeBufferState::DISPATCH;

	buffer.supply(testA, 22);
	dprint(testA);

	while(cState != RackoonIO::NormalisedSizeBufferState::PARTIAL) {
		// First check
		if(buffer.getState() == RackoonIO::NormalisedSizeBufferState::DISPATCH) {
			dispatch = buffer.dispatch();
			dprint(dispatch);
			continue;
		}

		// if we're here then the buffer is
		// partial
		
		if((cState = buffer.supply(testB, 27)) == RackoonIO::NormalisedSizeBufferState::DISPATCH) {
			dispatch = buffer.dispatch();
			dprint(dispatch);
		}
	}

}
