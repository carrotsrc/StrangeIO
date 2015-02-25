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
	g++ -I./ -std=c++11 -L./ -lpthread -lrackio test/memtest.cpp -o memltest

tests: librackio.so test/NormalisedSizeBuffer.cpp test/DelayBuffer.cpp test/LoadTelemetry.cpp
	g++ -I./ -std=c++11 -L./ -lpthread -lrackio test/NormalisedSizeBuffer.cpp -o bufltest;
	g++ -I./ -std=c++11 -L./ -lpthread -lrackio test/DelayBuffer.cpp -o delaybufltest;
	g++ -I./ -std=c++11 -L./ -lpthread -lrackio test/LoadTelemetry.cpp -o telemltest;

clean:
	find . -name "*.o" -type f -delete
