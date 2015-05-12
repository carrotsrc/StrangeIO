![](assets/RackoonIO.png?raw=true)

**Version: 0.2a**

**For Linux**

An object orientated virtual audio rack framework. It is designed around building reusable setups via a JSON configuration, describing daisychains of processing units, connected via virtual plugs and jack sockets. It is a hands on developer project.

- Dedicated MIDI control. Units can export functions into framework to allow for direct binding to MIDI signals
- Event driven design allowing for many ways in which cycle could be tweaked
- Mulithreaded with easy assignment of concurrent tasks enables samples to be processed in parallel
- Start of a library of general purpose buffers for handling samples
- Simple memory management of sample blocks

The framework is being built with ease of client development and exposing fine grain tweaking as a serious aim, which also means it needs to be extensible and modular.

#### Test Architecture

In the current test implementation on top of RackoonIO, the architecture is a three tier separation of concerns: 

- Framework
- Client library with implementation of environment, including processing units
- Client front end interface for environment start and control

## Hosting

Version 0.2a (current version) is the start of implementing hosting platform code paths for other plugin standards, to complement or extend native RackoonIO units.

#### LV2

Integrating an optional, and currently *basic*, hosting platform for LV2 plugins. Supplied LV2Adaptor class can be used as the basis for wrapping plugin functionality in a Unit, allowing it to be chained in the rack as a native unit.

## Rackscallion

There is also a small library of native processing units called [libRackscallion](https://github.com/carrotsrc/libRackscallion), which links directly with RackoonIO and can make a usable setup.

## Dependencies

**Required** ALSA / libasound

*Optional* LV2 and it's dependencies

## Building

There are three separate build scrips:
- framework/
- test/
- examples/

In each of the directories, configure the script using:

`$ cmake .`

Then once it is configured, run the make

`$ make`

## Running Examples

If the library wasn't installed, it's easiest to use the runex bash file in the examples/ directory, which will set LD_LIBRARY_PATH to the parent directory

Either way, you should use the command-line parameter ```-c [.cfg file]``` to specify which configuration file to load which could be impulse.cfg or sine.cfg

Example:

`./runex -c sine.cfg `

`./rackioex -c sine.cfg `

## Future plans

Among other things:

- Integrate more hosting platforms
- Expose more tweaks to the configuration file
- Make cross-platform -- core framework only depends on ALSA for MIDI
- Better C++ code

# Credit

The all important part where giving credit where it's due -
- [picoJSON](https://github.com/kazuho/picojson) which is an excellent library, used to load the configuration.
- ALSA for MIDI input
- [Exo Font](http://www.fontsquirrel.com/fonts/exo/) used in the logo
- [LV2](http://lv2plug.in/) for their quality plugin standard

These libraries are used in libRackscallion, but they are important so I'll mention them here as well -

- libResample (used in audacity) used for the experimental resampling unit
- libSndfile over at [MegaNerd](http://www.mega-nerd.com/libsndfile/) which is used for loading flacs.

## License

RackoonIO is licensed under LGPL v3


picoJSON is originally distributed under the terms of the 2-clause BSD license (see the license file in framework/picojson/LICENSE for details).

Optional LV2 is originally distributed under the ISC License (see license file in framework/hosting/licenses/LV2 for details)
