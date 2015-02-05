CPP=g++
CFLAGS=-ggdb -I./ -std=c++11
ASLIB=-fPIC -shared -Wl,-soname,librackio.so
LDFLAGS=`pkg-config --libs sndfile alsa` -lpthread
SOURCES := $(shell find ./ -name '*.cpp' ! -name '*entry.cpp')
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o: %.cpp
	$(CPP) $(CFLAGS) $(ASLIB) -c $< -o $@

librackio.so: $(OBJECTS)
	$(CPP) $(CFLAGS) $(ASLIB) $(LDFLAGS) $(OBJECTS) -o librackio.so

entry.o:
	$(CPP) $(CFLAGS) $(LDFLAGS) -L./ -lrackio -c entry.cpp  -o entry.o

rackio: entry.o
	$(CPP) $(CFLAGS) $(LDFLAGS) -L./ -lrackio entry.o  -o rackio

all: librackio.so
	make rackio
	

clean:
	find . -name "*.o" -type f -delete
