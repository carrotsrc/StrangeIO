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
class RuAlsa : public RackoonIO::RackUnit
{
	enum WorkState {
		IDLE,
		INIT,
		READY,
		PRIMING,
		STREAMING,
		FLUSHING,
		PAUSED
	};

	WorkState workState;
	snd_pcm_t *handle;
	unsigned int sampleRate, mLatency, bufSize, bufLevel, maxPeriods;
	RackoonIO::Buffers::DelayBuffer<short> *frameBuffer;
//	short *frameBuffer;
	snd_pcm_uframes_t triggerLevel, fPeriod;

	RackoonIO::FeedState feedJackAudio();
	std::mutex bufLock;

	void audioFeed();

	void actionInitAlsa();
	void actionFlushBuffer();


public:
	RuAlsa();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);
};

#endif
