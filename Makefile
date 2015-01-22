CPP=g++
CFLAGS=-ggdb -I./ -std=c++11
LDFLAGS=`pkg-config --libs sndfile alsa` -lpthread
SOURCES := $(shell find ./ -name '*.cpp')
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

all: $(OBJECTS)
	$(CPP) $(LDFLAGS) $(OBJECTS) -o rackio


clean:
	find . -name "*.o" -type f -delete
