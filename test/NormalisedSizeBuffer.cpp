#include "framework/buffers/NormalisedSizeBuffer.h"
#include "common.h"

void dprint(int *d) {
	cout << "---" << endl;
	for(int i = 0; i < 20; i++)
		cout << d[i] << " ";

	cout << endl;
}
int main( void ) {
	RackoonIO::NormalisedSizeBuffer<int> buffer(20, 16<<2);
	int testA[22], testB[27];
	int j = 1;
	for(int i = 0; i < 22; i++)
		testA[i] = j++;

	for(int i = 0; i < 27; i++)
		testB[i] = j++;

	cout << "Max value: " << j-1 << endl;

	int *dispatch;

	RackoonIO::NormalisedSizeBufferState cState = RackoonIO::NormalisedSizeBufferState::DISPATCH;

	buffer.supply(testA, 22);
	buffer.reset();

	while(cState != RackoonIO::NormalisedSizeBufferState::PARTIAL &&
	cState != RackoonIO::NormalisedSizeBufferState::OVERFLOW) {
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
		} else if(cState == RackoonIO::NormalisedSizeBufferState::OVERFLOW)
			cout << "Overflow Occurred" << endl;
	}

	cout << "Done!" << endl;

}
