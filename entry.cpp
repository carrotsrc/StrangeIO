#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
#include "framework/threads/RackQueue.h"

int main(void)
{
/*
	RuFlacLoad f;
	RuAlsa a;
	f.setConnection("audio_out", "audio", &a);
	f.setConfig("filename", "/home/charlie/Brain in a Fish Tank.flac");
	f.getJack("power")->rackFeed(RACK_AC);
*/
	RackoonIO::RackQueue pool(3);
	pool.init();
	int x;
	cin >> x;
}




