all: routines unitloading config cycling
	@echo "Built all"

routines: Routines.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o routines Routines.cpp

config: Config.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o config Config.cpp

unitloading: UnitLoading.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o unitloading UnitLoading.cpp

cycling: Cycling.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio units/TestingUnit.rso -o cycling Cycling.cpp

units: FORCE_UNITS
	unitbuild units/TestingUnit units/TestingUnit.cpp
	unitbuild units/MainlineUnit units/TestingUnit.rso units/MainlineUnit.cpp
	unitbuild units/OutputUnit units/TestingUnit.rso units/OutputUnit.cpp

FORCE_UNITS:
