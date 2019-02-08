#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <MLV/MLV_all.h>
#include <assert.h>

#include "algo_jeu.h"
#include "jeu_graphique.h" 




void dessine_ville_chemin(Lst_visite *liste_visite, Ville *liste_villes, const int nb_villes) {
    int x1, y1, x2, y2;
    int i;

    assert(liste_villes != NULL);
    assert(liste_visite != NULL);
    assert(nb_villes > 1);

    x1 = liste_visite[0].lst_ville_ordonne[0].x;
    y1 = liste_visite[0].lst_ville_ordonne[0].y;
    MLV_draw_filled_circle(x1, y1, 3, MLV_COLOR_RED); /* Dessine la premiere ville */

    for(i = 0; i < nb_villes - 1 ; ++i){   /* On retrace le chemin */
        x1 = liste_visite[0].lst_ville_ordonne[i].x;
        y1 = liste_visite[0].lst_ville_ordonne[i].y;
        x2 = liste_visite[0].lst_ville_ordonne[i+1].x;
        y2 = liste_visite[0].lst_ville_ordonne[i+1].y;
        MLV_draw_line(x1, y1, x2, y2, MLV_COLOR_GREEN);
        MLV_draw_filled_circle(x2, y2, 3, MLV_COLOR_RED);       /* Dessine autres villes */
    }

    MLV_actualise_window();

}




void affiche_jeu(Ville *liste_villes, const int nb_villes) {

    int boucle = 0;
    double distance_totale = 0;
    
    int nb_visite_alea = 128;
    Lst_visite liste_visite[nb_visite_alea];            /* Contient toutes les les listes visité 2à2 et leurs distance totale */
    Lst_visite liste_visite_copie[nb_visite_alea];

    assert(liste_villes != NULL);
    assert(nb_villes > 1);


    /* -------Visite generé alea------------ */
    gene_visite_alea(liste_visite, liste_villes, nb_visite_alea, 0, nb_villes);
    
    /* On trie V */
    qsort(liste_visite, nb_visite_alea, sizeof(*liste_visite), struct_cmp_par_distance);
    /* On copie V ds V' */
    memcpy(liste_visite_copie, liste_visite, sizeof(liste_visite_copie));




    MLV_create_window("Jeu", "texts and boxes", 600, 600);
    
    dessine_ville_chemin(liste_visite, liste_villes, nb_villes);

    /* ------------------- */
    while(1) {

        distance_totale = liste_visite[0].distance_totale;

        algo_jeu(liste_visite, liste_visite_copie, liste_villes, nb_visite_alea, nb_villes);

        if(distance_totale > liste_visite[0].distance_totale) { /* Si la distance est plus petite apres mutation */
            /* if (boucle % 100 == 0){ */
                MLV_clear_window( MLV_COLOR_BLACK );
                dessine_ville_chemin(liste_visite, liste_villes, nb_villes);
            /*} */
            ++boucle;
            printf("distance = %0.2f\n", sqrt(liste_visite[0].distance_totale));
            printf("\n");
        }
        
    }
}