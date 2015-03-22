This is the beginnings of a pure object orientated library for a virtual audio rack system. It's being designed as a framework around individual processing units, linked together by virtual connectors. There has always been a heavy influence of Reason in my head (since my brief play around) but more dedicated to mixing audio with added effects rather than producing it. It is also acting as a lab environment for my study of DSP.

There is Mixxx and Traktor out there for software mixing, but I sort of had in mind something less refined and more modular - like a rack of audio units... a virtual audio rack.

I also looked into GStreamer and it is stupendous. I did experiment with it, putting consideration into it but in the end I decided to look into designing something that is more focussed on what I had in mind. It has also been quite a learning experience building the pipelines.

----



## Design overview

Since the merge of callbackfw branch -- most of the system has shifted over to an event driven framework which has taken the basic CPU load from 9% down to <= 1% (tested with fairly trivial daisychains though). Now that cycling the rack is done by events, it's not doing 'idle' cycles but running when it is required. This may change if better designs emerge.

### Rack cycle

The RackoonIO::Rack object cycles whenever specific events are trigged - its cycle sends what is called an *AC* signal through the units that are plugged into its *mainlines*. This signal travels down the daisychain of units, and as it does so it wakes up each unit to perform a function like push data through to the next unit, receive data from previous unit or perform some data processing. Pretty straight forward but sounds like there might be plumbing problems - does it block waiting for processing to finish? The answer is yes. That is bad for (firm) real-time.

The current solution is to have the AC cycle being a controlled push through the rack but also to have as much processing pushed out to a thread pool. This is where the parallelism comes into play - since each unit is performing some sort of audio processing individually, it can push its work out to a worker thread so it crunches the data concurrently. The threaded unit can then push data through to the next unit once it is done processing and maybe trigger an event to cycle the rack again.

What happens if the next unit isn't ready to take more data? Well, it signals back with FEED_WAIT. If we're in a work thread it would be unwise to block the thread so we need to store the data and drop back to the rack cycle, waiting for the AC signal to wake us up again and try pushing the data through. What happens to queue data further upstream? Depending on the unit, it could continue filling up a buffer until it has reached it's limit or signal FEED_WAIT up through the daisychain; this signal can reach the file loader which haults any further reading until FEED_OK is signaled from the next unit.

It's best to think of it needing momentum to keep running, like an engine turning, so whenever a unit has finished processing something it seems best to trigger a rack cycle.

### Thread pool cycle

The RackQueue handles the threadpool. Whenever a unit whats to *outsource* data processing to a thread, it adds a new work package to the queue which in turns fires up the pump to dispatch the job to a thread. If there are no threads available to process the job, the queue will wait until one of the worker threads completes and signals that it is waiting for a new load. At the moment there is no priority - the pump cycles the jobs FIFO.

### MIDI cycle

The units are built to be controlled through MIDI input. This is handled in the main thread but I would like to shift it out to it's own thread. At the moment it cycles when the rack cycles - on each cycle it pulls the messages from the MIDI handle and routes them to the unit that is bound to that particular MIDI code. The unit is bound through callback, so a unit object "exports" a bunch of it's methods that can be bound to signals. How is the binding done? In a very straight forward way: via configuration!

### Configuration

Configuration is done through a JSON formatted file. This file specifies the setup of the rack, how many mainlines there are and the daisychains of units feeding off the mainlines; all described by a list of connections, followed by individual unit configurations. Different chains can by mixed together with a channel mixer. For instance, you may have two mainlines powering two file loaders - one channel for each track, which are then combined into a single channel using a channel mixer.

Units also have their own settings that you can set, including binding an exported method to a MIDI signal.

At the moment MIDI devices have aliases that they are assigned in the configuration based on their hw code.

The configuration file also specifies some framework settings, such as the number of threads in the pool, the number of microseconds of sleep for cycles, etc.

### Caching

Before merging the mem branch there was a lot of regular sized allocations occurring. Now there is a bitfield managed cache of blocks to avoid continuous allocations. The bitfield inherits from a more general RackoonIO::CacheHandler, meaning a different cache system can be implemented as a drop in, but the bitfield will work for now.

### Buffers

Having spent hours debugging buffers for handling sample IO in units (they have been the major headache), the library has the beginning of a collection of ready to use general purpose buffers.

### Telemetry

Have just started building in compiler conditions for including (or excluding) telemetry code paths. See the readme in the framework/telemetry/

## libBuccaneer

I have been working on a small library of processing units called [libBuccaneer](https://github.com/carrotsrc/libBuccaneer), which links directly with RackoonIO and can make a usable setup.

NOTE: This has not yet been modified to the new event based system.

## Dependencies

RackoonIO depends on ALSA.

## Inspiration

It's been a long time since the event but I was once watching a dub sound system and studied the guy as he fiddled around with hardware, listening to the sound loop, twist and fold in on itself and burst out into pure clarity. He looked like he was having a great time and I thought that was pretty cool! Then some years later I listened to the sound in 'King of the Sounds and Blues' by Zion Train and that planted a seed that's starting to grow... while a sound rig is waaaaay beyond budget (and my understanding), the more accessible approach would be to build a modular rack-style system for playing around with audio. This has also fed into my enjoyment of DSP.

# Credit

The all important part where giving credit where it's due -
- [picoJSON](https://github.com/kazuho/picojson) which is an excellent library, used to load the configuration.
- ALSA for MIDI input

These libraries are used in libBuccaneer, but they are important so I'll mention them here as well -

- libResample (used in audacity) used for the experimental resampling unit
- libSndfile over at [MegaNerd](http://www.mega-nerd.com/libsndfile/) which is used for loading flacs.

## License

This software is licensed under LGPL v3,

picoJSON is originally distributed under the terms of the 2-clause BSD license (see the license file in framework/picojson/LICENSE for details).

## Bug zapping tips

If the sound is not *crystal* clear through the speakers (usually, the smallest distortion is very easy to hear), then there is a bug; chances are there is something wrong with a buffer.

Sometimes if there is a problem in the pipeline, the sound has a very mild distortion -  it's tough to find out where it's occuring just by listening because the units are cycling at silly speeds, and a cause of distortion might be occurring at some small iterval like a millisecond:
- Dump the raw PCM and import it into Audacity, zooming in to get an idea of the distortion
- If the distortion seems to be regular (obvious sample drops, regular periods of 0s, etc), open the PCM in a hex editor to see actual byte sizes of these periods
- If you're not immediately locked in on the problem, it should at least give you an area to start concentrating on

If you know the general area of a problem but not sure where in the program flow it is occurring:
- Go to the problem area and start pushing different valued spikes or small square waves out at different points in the flow (e.g. before a buffer flush or an *if* statement)
    - You could just append the square wave onto the PCM dump on the final output
    - or you could overwrite data that is about to bed flushed out of the unit
- Import into Audacity to get visual feedback on the flow of the program, use a hex editor to get exact values
- Using this technique and a bit of trial and error (and repetition), it's possible to lock in on the exact sample where the problem is occuring

If you keep hearing high pitched clicks:
- Use the above techniques to find area, even down to the problem sample
    - If the problem sample is random, even on various repeated test then it's dodgy memory somewhere
    - If the sample value is regular then it's probably a stitching problem between two periods of samples

