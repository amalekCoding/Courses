#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "algo_jeu.h"
#include "jeu_graphique.h" 

#define NB_VILLES 1000




int main(int argc, char **argv) {
    int nb_villes;
    Ville *liste_villes = NULL;              /* Liste de toutes les villes */

    srand(time(NULL));

    printf("Donner un nb k de villes tel que : 1 < k <= %d.\n", NB_VILLES);
    do {
        printf("Donner le nb villes : ");
        scanf("%d", &nb_villes);
    } while(nb_villes <= 1 || nb_villes > NB_VILLES);


    liste_villes = gene_alea_villes(nb_villes);

    affiche_jeu(liste_villes, nb_villes);
    free(liste_villes);
    return 0;

}
