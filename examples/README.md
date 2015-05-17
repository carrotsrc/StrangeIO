# Examples

This directory holds some example units and example configuration files, with a client environment that fires up RackoonIO.

### Environment

The environment is the client that handles loading and initialising RackoonIO. You can see that the client provides two units in it's factory - Alsa and Impulse. There is also RuSine but this is dynamically loaded at runtime. You load the configuration up into the environment to setup the daisychains. Once it the rack goes through its warm up cycle, the units will start processing.

### Impulse

```./runex -c impulse.cfg```

Impulse loads the RuImpulse unit, chained to the Alsa output unit. It will send an impulse through the system every ```wait_time``` milliseconds at amplitude of ```impulse_value``` 

#### impulse.cfg

The configuration is an example of building a simple daisychain of units to create a functional setup. The configuration demonstrates labelling and setting config options for a unit. It is also loading units directly from the client environment rather than dynamically loading the units at run-time from a library.



### Sine

```./runex -c sine.cfg```

Sine loads the RuSine unit, and chains it to the Alsa output unit. The RuSine unit will generate a simple sine wave with centre frequency ```freq```

#### sine.cfg

The configuration shows more options. First we specify a midi device to be used as a controller because the RuSine unit is controllable. Looking at RuSine we can see by the ```library``` setting that the unit is not a part of the client environment but is dynamically loaded at runtime. The Library setting specified the rso shared object file. Next we can see that the ```freq``` function (which is an alias for an exported method in the unit) is directly bound to a MIDI signal -- the MIDI module and code to bind to the method. Whenever the system receives this code it will go straight to the exported method. The exported method steps the frequency by 2Hz around the centre frequency with correct phase.
