CC=gcc
CFLAGS=-ansi -pedantic -Wall 
OBJ= main.o graphique.o serpent.o case.o pomme.o monde.o deuxJoueur.o touche_jeu.o ia.o
LDFLAGS= -lMLV -lm

Projet: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

ia.o: ia.c ia.h monde.h serpent.h pomme.h case.h graphique.h touche_jeu.h deuxJoueur.h
	$(CC) -c ia.c $(CFLAGS)
deuxJoueur.o: deuxJoueur.c deuxJoueur.h monde.h serpent.h pomme.h case.h graphique.h touche_jeu.h 
	$(CC) -c deuxJoueur.c $(CFLAGS)
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
touche_jeu.o: touche_jeu.c touche_jeu.h case.h pomme.h serpent.h monde.h
	$(CC) -c touche_jeu.c $(CFLAGS)
main.o: main.c graphique.h serpent.h pomme.h case.h monde.h deuxJoueur.h touche_jeu.h ia.h
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