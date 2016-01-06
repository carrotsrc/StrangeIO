OBJ=catch_log.o catch_buffer.o catch_thread.o suite.o

all: suite
	@echo "Built all"

catch_buffer.o: catch_buffer.cpp
	g++ -ggdb -std=c++14 -I../framework/include/ -o catch_buffer.o -c catch_buffer.cpp -L../ -lstrangeio

catch_log.o: catch_log.cpp
	g++ -ggdb -std=c++14 -I../framework/include/ -o catch_log.o -c catch_log.cpp -L../ -lstrangeio
	
catch_thread.o: catch_thread.cpp
	g++ -ggdb -std=c++14 -I../framework/include/ -o catch_thread.o -c catch_thread.cpp -pthread  -lpthread

suite.o: suite.cpp
	g++ -ggdb -std=c++14 -I../framework/include/ -o suite.o -c suite.cpp -L../ -lstrangeio

suite: $(OBJ) 
	 g++ -ggdb -std=c++14 -L../ -lstrangeio -ldl $(OBJ) -pthread  -lpthread -o suite

units: FORCE_UNITS
	unitbuild unit03/BasicUnit unit03/BasicUnit.cpp
	unitbuild unit03/TestUnits unit03/Phi.cpp unit03/Tau.cpp
	unitbuild unit03/LinuxRig -lasound unit03/Theta.cpp unit03/Zeta.cpp


FORCE_UNITS:
	
clean:
	rm -f $(OBJ)
