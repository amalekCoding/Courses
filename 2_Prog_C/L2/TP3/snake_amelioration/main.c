#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <MLV/MLV_all.h>

#include "case.h"
#include "pomme.h"
#include "serpent.h"
#include "monde.h"
#include "deuxJoueur.h"
#include "graphique.h"
#include "ia.h"
#include "touche_jeu.h"






int main(){

    int nb_joueur, ia;
    Monde m;
    srand(time(NULL));
    m = init_monde(3);
 

    do{
        printf("Voulez vous jouer a 1 ou 2 joueur : \n");
        scanf("%1d", &nb_joueur);
    }while(nb_joueur < 1 || nb_joueur > 2);

    if (nb_joueur == 1){
        do{
            printf("Jouer contre la machine ?\n 0 = non | 1 = oui\n");
            scanf("%1d", &ia);
        }while(ia < 0 || ia > 1);
    }

    MLV_create_window("Snake", "", LARGEUR, HAUTEUR);
    if(nb_joueur == 1){
        afficher_monde(&m);
        MLV_actualise_window();
        
    }
    if (nb_joueur == 1 && ia){
        jeuIa(&m);
    }
    if(nb_joueur == 2){
        jeu_deux_joueur();
    }
    else{
        MLV_wait_keyboard(NULL, NULL, NULL);
        while(1){

            afficher_monde(&m);

            touche_jouer(&m, NULL, nb_joueur);

            MLV_wait_milliseconds(100);
            if (!deplacer_serpent(&m))
                exit(EXIT_FAILURE);

            MLV_clear_window( MLV_COLOR_BLACK );
            /*printf("dir = %d\n", m.serpent.dir);*/

        }
    }

    MLV_free_window();

    return 0;
}