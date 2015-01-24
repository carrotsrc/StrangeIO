#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
#include "framework/threads/ThreadPool.h"

int main(void)
{
	/*RuFlacLoad f;
	RuAlsa a;
	f.setConnection("audio_out", "audio", &a);
	f.setConfig("filename", "/home/charlie/Brain in a Fish Tank.flac");
	f.getJack("power")->rackFeed(RACK_AC);
*/
	RackoonIO::ThreadPool pool(6);
	pool.init();
	int x;
	cin >> x;
}




