#include "RuFlacLoad.h"

RuFlacLoad::RuFlacLoad()
: RackUnit() {
	addJack("phono1", 128);
	addJack("phono2", 128);
}
