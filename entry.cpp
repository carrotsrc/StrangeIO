#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
#include "framework/threads/RackQueue.h"

int main(void)
{

	RuFlacLoad f;
	RuAlsa a;
	f.setConnection("audio_out", "audio", &a);
	f.setConfig("filename", "/home/charlie/Brain in a Fish Tank.flac");
	f.getJack("power")->rackFeed(RackoonIO::RackState::RACK_AC);

	RackoonIO::RackQueue pool(6);

	pool.init();
	pool.cycle();
	int x;
	cin >> x;
}




