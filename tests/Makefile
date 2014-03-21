DEP_INCLUDE+=-I../ -I../utility/ -I../zlib/ -I/home/factoid/src/cpp/bandit/
CC=g++
CFLAGS=-std=gnu++11 -ggdb3 -O0 -fno-inline-small-functions -fmax-errors=10 -fPIC -Wall -Wno-unused-function

load_core.test: load_core.cpp libigc.so
	$(CC) $(CFLAGS) $(DEP_INCLUDE) -L../igc/ $< -o $@ -ligc

clean:
	rm *.test
