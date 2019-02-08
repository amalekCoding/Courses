#ifndef __AFFICHAGE_GRAPHIQUE__
#define __AFFICHAGE_GRAPHIQUE__

#include "algo_jeu.h"


void dessine_ville_chemin(Lst_visite *liste_visite, Ville *liste_villes, const int nb_villes);

void affiche_jeu(Ville *liste_villes, const int nb_villes);

void allouer(Lst_visite *liste_visite, const int nb_villes);


#endif


