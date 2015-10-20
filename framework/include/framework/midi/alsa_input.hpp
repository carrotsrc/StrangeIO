#ifndef __ALSAINPUTHANDLE_HPP_1440961572__
#define __ALSAINPUTHANDLE_HPP_1440961572__

#include <alsa/asoundlib.h>

#include "framework/fwcommon.hpp"
#include "framework/midi/midi.hpp"
#include "framework/midi/input_handle.hpp"

namespace strangeio {
namespace midi {

class alsa_input : public input_handle {
public:
	alsa_input(snd_rawmidi_t* handle, std::string port_name);
	msg flush();

	snd_rawmidi_t* handle();

private:
	snd_rawmidi_t* m_handle;
};

}
}
#endif