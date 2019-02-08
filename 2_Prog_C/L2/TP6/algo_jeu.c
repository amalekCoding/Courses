#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "algo_jeu.h"







int struct_cmp_par_distance(const void *a, const void *b) {
    /* Cmp par ordre croissant */
    Lst_visite *A = (Lst_visite *)a;
    Lst_visite *B = (Lst_visite *)b;

    return (A->distance_totale - B->distance_totale);
} 


double calcul_distance(Ville *v1, Ville *v2) {
    double distance;

    assert(v1 != NULL);
    assert(v2 != NULL);

    distance = pow(v1->x - v2->x, 2) + pow(v1->y - v2->y, 2);

    return distance;
}


int rand_a_b(int a, int b) {
    return rand() % (b - a) + a;
}

Ville* gene_alea_villes(const int nb_villes){
    int i;
    Ville *liste_villes;
    
    assert(nb_villes > 1);

    liste_villes = (Ville *) malloc(sizeof(Ville) * nb_villes);
    if(NULL == liste_villes)
        exit(EXIT_FAILURE);
    
    /* Initialisation de ville au hasard */
    for(i = 0 ; i < nb_villes ; ++i){ 
        liste_villes[i].x = rand()%600;
        liste_villes[i].y = rand()%600;
        liste_villes[i].est_visite = 0;
    }
    
    return liste_villes;
}

int modif_visite_mut(Lst_visite *liste_visite, const int alpha, const int nb_villes) {
    int i, j;
    double distance_totale = 0;

    int nb_visite = nb_villes - 1;

    assert(nb_villes > 1);
    assert(liste_visite != NULL);


    for (i = 0 ; i < alpha ; ++i) {
        for (j = 0 ; j < nb_visite; ++j) {
            liste_visite[i].distance_ville[j] = calcul_distance(&(liste_visite[i].lst_ville_ordonne[j]), &(liste_visite[i].lst_ville_ordonne[j + 1]));
            distance_totale += liste_visite[i].distance_ville[j];
        }

        liste_visite[i].distance_totale = distance_totale;

        distance_totale = 0;
    }

    return 1;
}


void gene_visite_alea(Lst_visite *liste_visite, Ville *liste_villes, const int nb_alea, const int beta, const int nb_villes) {
    int i, j;   
    double distance_totale = 0;

    int nb_visite = nb_villes - 1;

    int tmp_x = 0;
    int tmp_y = 0;
    int cpt = 0;


    assert(liste_visite != NULL);
    assert(liste_villes != NULL);

    /*  ------- Visite generé alea ------------ */
        /* gamma = nb_visite - beta */
    for (i = beta; i < nb_alea ; ++i) {
        /* 1er ville a voir */
        do {
            tmp_x = rand()%nb_villes;
        } while(liste_villes[tmp_x].est_visite == 1);
        liste_villes[tmp_x].est_visite = 1;
        liste_visite[i].lst_ville_ordonne[cpt] = liste_villes[tmp_x];
        ++cpt;

        for (j = 0 ; j < nb_visite; ++j) {
            /* 1er ville avec une 2em */
            do {
                tmp_y = rand()%nb_villes;
            } while(liste_villes[tmp_y].est_visite == 1);
            liste_villes[tmp_y].est_visite = 1;
            liste_visite[i].lst_ville_ordonne[cpt] = liste_villes[tmp_y];
            ++cpt;


            liste_visite[i].distance_ville[j] = calcul_distance(&(liste_villes[tmp_x]), &(liste_villes[tmp_y]));
            distance_totale += liste_visite[i].distance_ville[j];

            /* La 1er ville devient la 2em et ainsi de suite */
            tmp_x = tmp_y;
        }
        
        liste_visite[i].distance_totale = distance_totale;
        
        /* On renitialise pour la boucle */
        for(j = 0 ; j < nb_villes ; ++j) { 
            liste_villes[j].est_visite = 0;
        }

        distance_totale = 0;
        cpt = 0;
        /*        ---------------        */
    }

}



void algo_jeu(Lst_visite *liste_visite, Lst_visite *liste_visite_copie, Ville *liste_villes, const int nb_visite_alea, const int nb_villes) {

    Ville tmp_v;

    int i, j, k;
    int cpt = 0;

    int alpha = 64;
    int beta = 32 + alpha;        /* beta = alpha + 3 */


    int l = rand_a_b(1, nb_villes/2 + 1 );
    i = rand_a_b(1, nb_villes - (2 * l) + 2);
    j = rand_a_b(i + l, nb_villes - l + 2);

    assert(liste_visite != NULL);
    assert(liste_visite_copie != NULL);
    assert(liste_villes != NULL);
    assert(nb_villes > 1);


    /* Etape 1 */
    /* ------------- Mutations de V' ------------*/
   for(k = 0; k < alpha ; ++k){
        for (cpt = 0 ; cpt < l - 1 ; ++cpt) {
            tmp_v = liste_visite_copie[k].lst_ville_ordonne[i + cpt];
            liste_visite_copie[k].lst_ville_ordonne[i + cpt] = liste_visite_copie[k].lst_ville_ordonne[j + cpt];
            liste_visite_copie[k].lst_ville_ordonne[j + cpt] = tmp_v;
        }       
    }

    /* On reactualise la distance et les visite apres mutations */
    modif_visite_mut(liste_visite_copie, alpha, nb_villes);


    /* Etape 2 */
    /* On rajoute les premiers visite normale */
    for(k = alpha, cpt = 0 ; k < beta /* beta= beta-alpha */; ++k, ++cpt){
        liste_visite_copie[k] = liste_visite[cpt];
    }

    /* Etape 3 */
    /* -------Visite generé alea------------ */
    gene_visite_alea(liste_visite_copie, liste_villes, nb_visite_alea, beta, nb_villes);


    /* On copie V' ds V et le trie */
    qsort(liste_visite_copie, nb_visite_alea, sizeof(*liste_visite), struct_cmp_par_distance);
    memcpy(liste_visite, liste_visite_copie, sizeof(*liste_visite_copie));
    /* -----        --      ---     */

}

