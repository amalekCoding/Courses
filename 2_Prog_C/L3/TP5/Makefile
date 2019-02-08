CC=gcc
CFLAGS= -ansi -Wall -Werror -pedantic -O2
OBJ= main.o code.o
LDFLAGS= 

Tp5: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)



main.o: main.c code.h

%.o: %.c
	$(CC) -c $< $(CFLAGS)





clean: 
	rm -f main.o

mrproper: clean
	rm -f Tp5



