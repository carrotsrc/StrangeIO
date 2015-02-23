This is the beginnings of a pure object orientated library for a virtual audio rack system. It's being designed as a framework around individual processing units, linked together by virtual connectors. There has always been a heavy influence of Reason in my head (since my brief play around) but more dedicated to mixing audio with added effects rather than producing it. It is also acting as a lab environment for my study of DSP.

There is Mixxx and Traktor out there for software mixing, but I sort of had in mind something less refined and more modular - like a rack of audio units... a virtual audio rack.

I also looked into GStreamer and it is stupendous. I did experiment with it, putting consideration into it but in the end I decided to look into designing something that is more focussed on what I had in mind. It has also been quite a learning experience building the pipelines.


## Design overview

### Rack cycle

Keeping in mind it is currently in early prototype stages so design will change but currently how it functions (in a completely unoptimised way, there has been no testing for optiminal values) is a bit like a steam engine or some kind of pressurised system but with extra parallelism.

The RackoonIO::Rack object cycles on it's own thread (currently some silly-small amount of time like a hundred or so microseconds) and as it cycles it sends what is called an *AC* signal through the units that are plugged into it's *mainlines*. This signal travels down the daisychain of units, and as it does so it wakes up each unit to perform a function like push data through to the next unit, receive data from previous unit or perform some data processing. Pretty straight forward but sounds like there might be plumbing problems - does it block waiting for processing to finish? The answer is yes. That is bad for (firm) real-time.

The current solution is to have the AC cycle being the consistant and controlled push through the rack but also to have as much processing pushed out to a thread pool. This is where the parallelism comes into play - since each unit is performing some sort of audio processing individually, it can push its work out to a worker thread which acts as the unit running in parralel. The threaded unit can then push data through to the next unit once it is done processing.

What happens if the next unit isn't ready to take more data? Well, it signals back with FEED_WAIT. If we're in a work thread it would be unwise to block the thread so we need to store the data and drop back to the rack cycle, waiting for the AC signal to wake us up again and try pushing the data through. What happens to queue data further upstream? Depending on the unit, it could continue filling up a buffer until it has reached it's limit or signal FEED_WAIT up through the daisychain; this signal can reach the file loader which haults any further reading until FEED_OK is signaled from the next unit.

So it's a weird combination, a stream engine with parallel computation.

### Thread pool cycle

As well as the AC signal sent down the daisychain of units, RackoonIO::Rack object also tells the *RackoonIO::Threadpool* to cycle. When a processing unit wants to *outsource* work to a thread, it passes the work package to the pool queue. On every pool cycle, the work packages get distributed to the worker threads which then process them on their new cycle. At the moment, they sleep for a time and then check for more work rather than waking up on a signal, although I would like to test the latter at some point. So when the thread has a job, it runs the callback to the original unit's object, taking over the processing. So essentially, you are moving a particular object's method into its own thread - this has the benefit of allowing each unit to run in parallel.

### MIDI cycle

The units are built to be controlled through MIDI input. This is handled in the main thread but I would like to shift it out to it's own thread. At the moment it works like the rack and the thread pool - on each cycle it pulls the messages from the MIDI handle and routes them to the unit that is bound to that particular MIDI code. The unit is bound through callback, so a unit object "exports" a bunch of it's methods that can be bound to signals. How is the binding done? In a very straight forward way: via configuration!

### configuration

Configuration is done through a JSON formatted file. This file specifies the setup of the rack, how many mainlines there are and the daisychains of units feeding off the mainlines; all described by a list of connections, followed by individual unit configurations. Different chains can by mixed together with a channel mixer. For instance, you may have two mainlines powering two file loaders - one channel for each track, which are then combined into a single channel using a channel mixer.

Units also have their own settings that you can set, including binding an exported method to a MIDI signal.

At the moment MIDI devices have aliases that they are assigned in the configuration based on their hw code.

The configuration file also specifies some framework settings, such as the number of threads in the pool, the number of microseconds of sleep for cycles, etc.

## libBuccaneer

I have been working on a small library of processing units called [libBuccaneer](https://github.com/carrotsrc/libBuccaneer), which links directly with RackoonIO and can make a usable setup.

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
