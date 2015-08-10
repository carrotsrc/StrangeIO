all: routines config
	echo "Built all"

routines: Routines.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o routines Routines.cpp

config: Config.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o config Config.cpp
