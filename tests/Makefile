DEP_INCLUDE+=-I../ -I../utility/ -I../zlib/
CC=g++
CFLAGS=-std=gnu++11 -ggdb -O0 -fmax-errors=10 -fPIC

load_core.test: load_core.cpp libigc.so
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -L../igc/ $< -o $@ -ligc

clean:
	rm *.test
