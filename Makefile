CPP=g++
CFLAGS=-ggdb -I./ -std=c++11

LIBCFLAGS=$(CFLAGS) -fPIC -shared
LIBLDFLAGS=-Wl,-soname,librackio.so `pkg-config --libs alsa` -lpthread

LIBSOURCES := $(shell find ./framework -name '*.cpp' ! -name '*entry.cpp')
LIBOBJECTS=$(patsubst %.cpp, %.o, $(LIBSOURCES))

$(LIBOBJECTS): %.o: %.cpp
	$(CPP) $(LIBCFLAGS) -c $< -o $@

librackio.so: $(LIBOBJECTS)
	$(CPP) $(LIBCFLAGS) $(LIBLDFLAGS) $(LIBOBJECTS) -o librackio.so

all:
	make librackio.so
	
memtest: librackio.so memtest.cpp
	g++ -I./ -std=c++11 -L./ -lpthread -lrackio memtest.cpp -o memtest

buftest: librackio.so test/NormalisedSizeBuffer.cpp
	g++ -I./ -std=c++11 -L./ -lpthread -lrackio test/NormalisedSizeBuffer.cpp -o buftest;

clean:
	find . -name "*.o" -type f -delete
