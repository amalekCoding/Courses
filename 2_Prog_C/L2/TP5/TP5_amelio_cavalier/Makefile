CC=gcc
CFLAGS= -ansi -Wall
OBJ= main.o jeu.o affichage_graphique.o jeu_graphique.o
LDFLAGS= -lMLV

Projet: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)


jeu.o: jeu.c jeu.h

affichage_graphique.o: affichage_graphique.h affichage_graphique.h 

jeu_graphique.o: jeu_graphique.c jeu_graphique.h

main.o: main.c jeu.h affichage_graphique.h jeu_graphique.h

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean: 
	rm -f *.o

mrproper: clean
	rm -f Projet

install: Projet
	mkdir ../bin
	mv Projet ../bin../Projet
	make mrproper

uninstall: mrproper
	rm -f ../bin/Projet
	rm -rf ../bin