CPP=g++
CFLAGS=-ggdb -I./ -std=c++11

LIBCFLAGS=$(CFLAGS) -fPIC -shared
LIBLDFLAGS=-Wl,-soname,librackio.so `pkg-config --libs alsa` -lpthread

LIBSOURCES := $(shell find ./framework -name '*.cpp' ! -name '*entry.cpp')
LIBOBJECTS=$(patsubst %.cpp, %.o, $(LIBSOURCES))

BINSOURCES := $(shell find ./  -name '*.cpp' ! -path "./framework/*")
BINOBJECTS=$(patsubst %.cpp, %.o, $(BINSOURCES))
BINCFLAGS=$(CFLAGS) -L./ -lrackio

BINLDFLAGS=`pkg-config --libs sndfile alsa` -lpthread

$(LIBOBJECTS): %.o: %.cpp
	$(CPP) $(LIBCFLAGS) -c $< -o $@

librackio.so: $(LIBOBJECTS)
	$(CPP) $(LIBCFLAGS) $(LIBLDFLAGS) $(LIBOBJECTS) -o librackio.so


$(BINOBJECTS): %.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

rackio: $(BINOBJECTS)
	$(CPP) $(BINLDFLAGS) $(BINCFLAGS) $(BINOBJECTS)  -o rackio

all: librackio.so
	make rackio
	

clean:
	find . -name "*.o" -type f -delete
