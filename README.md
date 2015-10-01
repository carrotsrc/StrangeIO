![](assets/strangeio.png?raw=true)

**Version: 0.3**

**Primarily Linux**

StrangeIO is a modern C++ framework for building a virtual audio rack. The most recent revision has seen a complete overhaul in design and implementation, leading to a consistant style and a familiar feel as a C++ library, as well as improvements in how it works.

It is being designed for near real-time audio processing, so far tested within the 7 millisecond range and still got a lot of breathing room. The system has managed memory cache for handling the samples and is influenced by the STL smart pointers.

It has a cycle that is easily reasoned with - when samples are needed they are pushed down, processed through the daisychains and sent out, followed by a resync of the units if necessary. We utilise the latency of the output buffer to perform the resyncing.


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
