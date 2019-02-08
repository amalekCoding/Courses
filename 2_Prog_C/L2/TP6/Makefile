CC=gcc
CFLAGS= -ansi -Wall -Werror -O2
OBJ= main.o algo_jeu.o jeu_graphique.o
LDFLAGS= -lMLV -lm

Projet: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

algo_jeu.o: algo_jeu.c algo_jeu.h

affichage_graphique.o: jeu_graphique.c jeu_graphique.h algo_jeu.h

main.o: main.c jeu_graphique.h algo_jeu.h

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