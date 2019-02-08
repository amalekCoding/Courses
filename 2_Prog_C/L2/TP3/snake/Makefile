CC=gcc
CFLAGS=-ansi -pedantic -Wall 
OBJ= main.o graphique.o serpent.o case.o pomme.o monde.o 
LDFLAGS= -lMLV

Projet: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)


graphique.o: graphique.c graphique.h monde.h serpent.h pomme.h case.h
	$(CC) -c graphique.c $(CFLAGS)
serpent.o: serpent.c serpent.h case.h
	$(CC) -c serpent.c $(CFLAGS)
pomme.o: pomme.c pomme.h case.h
	$(CC) -c pomme.c $(CFLAGS)
case.o: case.c case.h
	$(CC) -c case.c $(CFLAGS)
monde.o: monde.h monde.c pomme.h serpent.h case.h
	$(CC) -c monde.c $(CFLAGS)
main.o: main.c graphique.h serpent.h pomme.h case.h monde.h
	$(CC) -c main.c $(CFLAGS)



%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean: 
	rm -f *.o
	rm -f *.gch

mrproper: clean
	rm -f Projet

install: Projet
	mkdir ../bin
	mv Projet ../bin../Projet
	make mrproper

uninstall: mrproper
	rm -f ../bin/Projet
	rm -rf ../bin