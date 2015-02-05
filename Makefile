CPP=g++
CFLAGS=-ggdb -I./ -std=c++11
ASLIB=-fPIC -shared -Wl,-soname,librackio.so
LDFLAGS=`pkg-config --libs sndfile alsa` -lpthread

LIBSOURCES := $(shell find ./framework -name '*.cpp' ! -name '*entry.cpp')
LIBOBJECTS=$(patsubst %.cpp, %.o, $(LIBSOURCES))

BINSOURCES := $(shell find ./  -name '*.cpp' ! -path "./framework/*")
BINOBJECTS=$(patsubst %.cpp, %.o, $(BINSOURCES))
BINCFLAGS=-L./ -lracio

$(LIBOBJECTS): %.o: %.cpp
	$(CPP) $(CFLAGS) $(ASLIB) -c $< -o $@

librackio.so: $(LIBOBJECTS)
	$(CPP) $(CFLAGS) $(ASLIB) $(LIBOBJECTS) -o librackio.so


$(BINOBJECTS): %.o: %.cpp
	$(CPP) $(CFLAGS) $(BINCFLAGS) -c $< -o $@

rackio: $(BINOBJECTS)
	$(CPP) $(CFLAGS) $(LDFLAGS) -L./ -lrackio $(BINOBJECTS)  -o rackio

all: librackio.so
	make rackio
	

clean:
	find . -name "*.o" -type f -delete
