#ifndef __ALSAINPUTHANDLE_HPP_1440961572__
#define __ALSAINPUTHANDLE_HPP_1440961572__

#include <alsa/asoundlib.h>

#include "framework/fwcommon.hpp"
#include "framework/midi/Midi.hpp"
#include "framework/midi/MidiInputHandle.hpp"

namespace strangeio {
namespace Midi {

class AlsaInputHandle : public MidiInputHandle {
public:
	AlsaInputHandle(snd_rawmidi_t* handle, std::string port_name);
	MidiCode flush();

	snd_rawmidi_t* handle();

private:
	snd_rawmidi_t* m_handle;
};

}
}
#endif