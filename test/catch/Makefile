all: units routines unitloading config
	echo "Built all"

routines: Routines.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o routines Routines.cpp

config: Config.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o config Config.cpp

unitloading: UnitLoading.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o unitloading UnitLoading.cpp

units: units/MainlineUnit.cpp units/OutputUnit.cpp
	unitbuild units/MainlineUnit units/MainlineUnit.cpp
	unitbuild units/OutputUnit units/OutputUnit.cpp
