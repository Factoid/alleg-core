DEP_INCLUDE+=-I../ -I../utility/ -I../zlib/
CC=g++
CFLAGS=-std=gnu++11 -ggdb -O0 -fmax-errors=10 -fPIC

load_core: load_core.cpp
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -L../igc/ load_core.cpp -o load_core -ligc
