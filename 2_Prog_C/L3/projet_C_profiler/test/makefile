CC= gcc
CFLAGS=-Werror -Wall -I../include
LDFLAGS=

all: test

test: test.o
	$(CC) -o $@ $^ $(LDFLAGS)

test.o: test.c ../include/macro_profiler.h
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -rf *.o
	rm -rf *.log

mrproper:
	rm -rf test
