#ifndef __ALSAOUTPUTHANDLE_HPP_1440961572__
#define __ALSAOUTPUTHANDLE_HPP_1440961572__

#include <alsa/asoundlib.h>

#include "framework/fwcommon.hpp"
#include "framework/midi/midi.hpp"
#include "framework/midi/output_handle.hpp"

namespace strangeio {
namespace midi {

class alsa_output : public output_handle {
public:
	alsa_output(snd_rawmidi_t* handle, std::string port_name);
	void write(msg chr);

	snd_rawmidi_t* handle();

private:
	snd_rawmidi_t* m_handle;
};

}
}
#endif