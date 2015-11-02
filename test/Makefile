OBJ=catch_thread.o

all: suite
	@echo "Built all"
	
catch_thread.o: catch_thread.cpp
	g++ -ggdb -std=c++14 -I../framework/include/ -L../ -lstrangeio -ldl -o catch_thread.o -c catch_thread.cpp -pthread  -lpthread
	
suite: $(OBJ) suite.cpp suite.hpp 
	g++ -ggdb -std=c++14 -I../framework/include/ -L../ -lstrangeio -ldl -o suite suite.cpp $(OBJ) -pthread  -lpthread

units: FORCE_UNITS
	unitbuild unit03/BasicUnit unit03/BasicUnit.cpp
	unitbuild unit03/TestUnits unit03/Phi.cpp unit03/Tau.cpp
	unitbuild unit03/LinuxRig -lasound unit03/Theta.cpp unit03/Zeta.cpp


FORCE_UNITS:
	
clean:
	rm -f $(OBJ)
