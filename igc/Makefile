DEP_INCLUDE+=-I../ -I../utility/ -I../zlib/
CC=g++
CFLAGS=-std=gnu++11 -ggdb3 -O0 -fno-inline-small-functions -fmax-errors=10 -fPIC -Wall -Werror -Wno-unused-function

sources = $(wildcard *.cpp)
objects = $(sources:.cpp=.o)
depends = $(sources:.cpp=.d)

all: libigc.so $(objects)

libigc.so: $(objects)
	$(CC) $(CFLAGS) -shared -o $@ $^

%.d: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -MM -o $@ $<

%.o: %.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -c $< -o $@

deps: $(depends)

install:
	cp libigc.so ../tests

depclean:
	rm *.d

clean:
	rm *.o *.so
