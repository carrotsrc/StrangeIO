/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef RUALSA_H
#define RUALSA_H
#include "framework/rack/RackUnit.h"
#include "framework/buffers/DelayBuffer.h"
namespace ExampleCode {
/** The unit that interfaces with ALSA
 *
 * This is used by any configuration that
 * requires source card output via libasound
 * and the ALSA interface.
 *
 * This unit takes a stream of frames and
 * stored them in a delay buffer. When the
 * buffer in ALSA has dropped down to a threshold
 * load, it flushes the delay buffer into the 
 * ALSA handle.
 */
class RuAlsa : public RackoonIO::RackUnit
{

public:
	/** The different states for the unit */
	enum WorkState {
		IDLE, ///< Unitialised
		INIT, ///< Initialising the unit
		READY, ///< Ready to receive data
		PRIMING, ///< Priming the delay buffer
		STREAMING, ///< Loading delay buffer
		FLUSHING, ///< Flushing the delay buffer
		PAUSED ///< Received a pause state
	};

	RuAlsa();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);

private:
	WorkState workState; ///< Current state of the unit
	snd_pcm_t *handle; ///< Alsa handle
	unsigned int sampleRate, ///< Sample rate of stream
		     bufSize, ///< The size of the delay buffer in frames
		     maxPeriods; ///< The maximum number of periods that can be stored in the Alsa buffer
	RackoonIO::Buffers::DelayBuffer<short> *frameBuffer; ///< The delay buffer

	snd_pcm_uframes_t triggerLevel, ///< threshold to flush the current buffer into ALSA
			  fPeriod; ///< The size of the period in frames

	RackoonIO::FeedState feedJackAudio();
	std::mutex bufLock; ///< thread ;ock on the delay buffer

	FILE *fp; ///< PCM dump - useful for this example code or debuggin

	void audioFeed();

	void actionInitAlsa();
	void actionFlushBuffer();
};
}
#endif
