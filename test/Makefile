SRC=catch_combine.cpp 
all: suite
	@echo "Built all"
suite: suite.cpp suite.hpp $(SRC)
	g++ -ggdb -std=c++11 -I../framework/include/ -L../ -lstrangeio -ldl -o suite suite.cpp $(SRC) -pthread  -lpthread

units: FORCE_UNITS
	unitbuild unit03/BasicUnit unit03/BasicUnit.cpp
	unitbuild unit03/TestUnits unit03/Phi.cpp unit03/Tau.cpp
	unitbuild unit03/LinuxRig -lasound unit03/Theta.cpp unit03/Zeta.cpp

FORCE_UNITS:
