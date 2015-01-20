CC=g++ -ggdb
CFLAGS=-I./
LDFLAGS=`pkg-config --libs sndfile alsa` -lpthread
SOURCES := $(shell find ./ -name '*.cpp')
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

all: $(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJECTS) -o rackio


clean:
	find . -name "*.o" -type f -delete
