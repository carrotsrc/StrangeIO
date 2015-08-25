all: routines objects unitloading config threadpool cycling eventloop
	@echo "Built all"

routines: Routines.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o routines Routines.cpp

objects: Objects.cpp Objects.hpp
	g++ -ggdb -std=c++11 -I../../framework/include/ -L../../ -lstrangeio -o objects Objects.cpp

config: Config.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o config Config.cpp

unitloading: UnitLoading.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o unitloading UnitLoading.cpp

threadpool: ThreadPool.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio -o threadpool ThreadPool.cpp -pthread

eventloop: EventLoop.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio units/TestingUnit.rso -o eventloop EventLoop.cpp -pthread

cycling: Cycling.cpp
	g++ -ggdb -std=c++11 -I../../ -L../../ -lstrangeio units/TestingUnit.rso -o cycling Cycling.cpp


units: FORCE_UNITS
	unitbuild units/TestingUnit units/TestingUnit.cpp
	unitbuild units/MainlineUnit units/TestingUnit.rso units/MainlineUnit.cpp
	unitbuild units/OutputUnit units/TestingUnit.rso units/OutputUnit.cpp

FORCE_UNITS:
