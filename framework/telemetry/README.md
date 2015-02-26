 # Telemetry
 
 Currently the process is fairly straight forward:
 
 ### Telemetry Objects
 
 These objects are built for specific parts of the framework. They setup callbacks in the object, which are then called to pass types of metrics back for processing.
 
 ### Code Paths
 
 The Telemetry system is being designed to be optional at compile time. The goal is to make the syntax for the compilation conditions as unintrusive as possible *within* functional code (i.e. not necessary in declarations like class headers). Since all the telemetry is being designed through the use of callbacks, it's easy to build a macro system which will drop conditional code at compile time in the function bodies, while avoiding eye-bleeding #if #endif.
 
As an example of a conditional code path:
RACK_TELEMETRY(callback, values...)

Will not be included in the final binary if the metrics for the Rack are not turned on when compiled. The macro is nice and unintrusive in function bodies. Each telemetry object will have a respective macro of the conditonal code.

 
 ### RackTelemetry
 
 Currently there is a measuring object for the Rack, with a UnitCycle metric, resulting in processing the duration of rack unit cycles.
 
 What I've found on my system so far is there is a critical warm up cycle (usually about 60-70 milliseconds), then after that it *averages* down to about 5 microsecond cycles when idling, and about 15 microsecond cycles when processing two channels, mixers and effects. These metrics shows the performance of the (weakly) controlled *AC* cycle without it's sleep time. This, however, is not delving into the times of all the parallel computations.
 
 Note: Overflows will result in a reset of relevent counters
