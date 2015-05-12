#include <math.h>
#include "HostingLv2.h"
#include "framework/memory/BitfieldCache.h"

using namespace std;

#define FS 512

PcmSample *generate_sample_array(uint32_t fs) {
	return (PcmSample*) malloc(sizeof(PcmSample)*fs);
}

void generate_sine(uint32_t fs, PcmSample **in) {
	auto ts = 1.0f/fs;

	for(int i = 0; i < 512; i++) {
		(*in)[i] = sin(2 * M_PI * ts * i);
	}
}

int main( void ) {
	RackoonIO::Hosting::LV2Platform platform;
	auto factory = new TestFactory();
	auto cache = new BitfieldCache();
	cache->init(FS, 8);
	factory->setLV2Platform(&platform);
	factory->setCacheHandler(cache);


	auto unit = factory->build("TestAdaptor", "TestAdaptor1");
	auto terminal = factory->build("Terminal", "Terminal1");

	unit->init();
	terminal->init();

	unit->setConnection("Output", "audio_in", terminal.get());

	auto sine = cache->alloc(1);
	generate_sine(FS, &sine);


	auto jack = unit->getJack("Input");
	jack->frames = 512;
	jack->feed(sine);

	cout << "Successful" << endl;
}
