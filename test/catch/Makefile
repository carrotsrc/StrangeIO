all: objects
	@echo "Built all"
objects: Objects.cpp Objects.hpp
	g++ -ggdb -std=c++11 -I../../framework/include/ -L../../ -lstrangeio -ldl -o objects Objects.cpp -pthread  -lpthread

units: FORCE_UNITS
	unitbuild unit03/BasicUnit unit03/BasicUnit.cpp
	unitbuild unit03/TestUnits unit03/Phi.cpp unit03/Tau.cpp
	unitbuild unit03/Phi unit03/Phi.cpp
	unitbuild unit03/Tau unit03/Tau.cpp

	unitbuild unit03/Rigging -ggdb -lasound unit03/Theta.cpp unit03/Zeta.cpp

FORCE_UNITS:
