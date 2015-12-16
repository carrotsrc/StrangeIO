![](assets/strangeio.png?raw=true)

**Version: 0.3**

**Primarily Linux**

StrangeIO is a modern C++ framework for building a virtual audio rack. The most recent revision has seen a complete overhaul in design and implementation, leading to a consistant style and a familiar feel as a C++ library, as well as improvements in how it works.

It is being designed for near real-time audio processing within the limits of perception, so far tested within the 7 millisecond range and still got a lot of breathing room. The system has managed memory cache for handling the samples and is influenced by the STL smart pointers.

It has been tested live at an event successfully through the use of StrangePad.

### Features
- STL influenced managed memory
- Dedicated listener for midi devices, routing signals directly to units
- Easy assignment of parallel tasks
- Optional event loop
- Dynamic unit loading
- A consistant and reasonable cycle of processing

## Todo

### Architecture

#### Automatically profile into parallel execution

There are concurrent elements to the pipeline -- for instance where many channels combine into a single channel through a Combine unit (A mixer, for example). This functions perfectly in a single threaded environment where a Combine unit's behaviour is to signal back partial cycles until all channels are combined, at which point it pushes the samples down the daisychain. Given the distinct behaviour of units, it should be possible to automatically run a profile of the daisychains, splitting individual channels, up to the Combine unit, into parallel tasks.

#### Locking to driver latency

When units sync, it is meant in a general sense -- they are syncing various aspects of their profile, from structural latency built into the line (i.e. number of period buffers downstream), to drift caused by speed shifts downstream, to audio profiles like Fs or number of channels. What needs to be tighter is locking units to the actual latency of the driver; given the average unoptimised downstream part of the cycle (i.e. the part that processes samples) takes aprox 150 microseconds with current StrangePad units, it seems reasonsable to read the latency from the driver API and feed it back up the units, a process which will take far less time than the downstream cycle and will provide a latency sync with a much smaller margin of error.

A potential design is to just have latency syncing on the return journey down the call stack. This does not add any new process since it is the natural result of reaching the end of the pipeline but the disadvantage is that there will be two forms of syncing -- one structural and the other more specific to driver latency. With this design -- what happens at Combine units? The problem is only one channel is active in the callstack when it returns. How does a combine sync the other channels?

Another potential design is for syncing to be two-way and provide an upstream sync solution which can be performed at the Dispatch unit. The advantage to this is that Combine unit behaviour is easily built in but the disadvantage is that it provides a whole process of syncing to deal with.

### Hosting

The progress with hosting is currently in staging, waiting to be updated to the current 0.3 interface. The ideal being to provide hosting platforms for:

- LV2 (Staging)
- Jack (Early design)
- Python (Starting design)

The Python is going to be very interesting as it will provide a live, scriptable interface for units.


# Credit

The all important part where giving credit where it's due -

- [picoJSON](https://github.com/kazuho/picojson) which is an excellent library, used to load the configuration.
- [Catch](https://github.com/philsquared/Catch/) the straight forward, easy to use testing framework
- [ALSA](http://www.alsa-project.org/) for MIDI input
- [Sansation Font](http://www.fontsquirrel.com/fonts/sansation) used in the logo as base font
- [LV2](http://lv2plug.in/) for their quality plugin standard
- [JACK](http://jackaudio.org/) another great standard, which is slowly getting to the point of being hosted in StrangeIO

## License

StrangeIO is licensed under LGPL v3


picoJSON is originally distributed under the terms of the 2-clause BSD license (see the license file in framework/picojson/LICENSE for details)
Catch testing framework is distributed under the terms of the Boost Software License (see the license file in tests/catch/LICENSE for details)

Optional LV2 is originally distributed under the ISC License (see license file in framework/hosting/licenses/LV2 for details)