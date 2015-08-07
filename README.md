![](assets/strangeio.png?raw=true)

**Version: 0.2a**

**For Linux**

An object orientated virtual audio rack framework. It is designed around building reusable setups via a JSON configuration, describing daisychains of processing units, connected via virtual plugs and jack sockets. It is a hands on developer project.

- Dedicated MIDI control. Units can export functions into framework to allow for direct binding to MIDI signals
- Event driven design allowing for many ways in which cycle could be tweaked
- Mulithreaded with easy assignment of concurrent tasks enables samples to be processed in parallel
- Start of a library of general purpose buffers for handling samples
- Simple memory management of sample blocks
- Easily build dynamically loaded units or sets of units

The framework is being built with ease of client development and exposing fine grain tweaking as a serious aim, which also means it needs to be extensible and modular.

## Dynamic Loading

As of version 0.2a -- dynamic loading has been integrated into the framework so units can be loaded up at runtime without needing to be linked with the client at compile time. This enabling decoupling the RackUnit object files from framework.

## Hosting

Version 0.2a (current version) is the start of implementing hosting platform code paths for other plugin standards, to complement or extend native StrangeIO units.

#### LV2

Integrating an optional, and currently *basic*, hosting platform for LV2 plugins. Supplied LV2Adaptor class can be used as the basis for wrapping plugin functionality in a Unit, allowing it to be chained in the rack as a native unit.

## Renaming

This project is now the framework behind StrangePad so has been changed to StrangeIO. The RackoonIO name served it well, but it is time for it to mature, and become coherent!

## Dependencies

**Required** ALSA / libasound

*Optional* LV2 and it's dependencies

## Building

To build the 
- framework/
- test/


In the framework directory run the cmake config command

`$ ccmake .`

Then once it is configured, run the make

`$ make`

The `make` command is used in the test directories

## Future plans

Among other things:

- Integrate more hosting platforms including runtime python
- Expose more tweaks to the configuration file
- Make cross-platform -- core framework only depends on ALSA for MIDI
- Better C++ code

# Credit

The all important part where giving credit where it's due -
- [picoJSON](https://github.com/kazuho/picojson) which is an excellent library, used to load the configuration.
- ALSA for MIDI input
- [Exo Font](http://www.fontsquirrel.com/fonts/exo/) used in the logo
- [LV2](http://lv2plug.in/) for their quality plugin standard

## License

StrangeIO is licensed under LGPL v3


picoJSON is originally distributed under the terms of the 2-clause BSD license (see the license file in framework/picojson/LICENSE for details).

Optional LV2 is originally distributed under the ISC License (see license file in framework/hosting/licenses/LV2 for details)
