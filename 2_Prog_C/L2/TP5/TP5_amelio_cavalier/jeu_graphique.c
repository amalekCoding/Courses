#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <MLV/MLV_all.h>
#include "jeu_graphique.h"
#include "affichage_graphique.h"

#define LARGEUR 600
#define PIXEL ((LARGEUR - 50) / 8)



/* Permet de recupere l'indice de la case du clic ou d'annuler un coup
    Retourne  0 : si non valide 
              1 : sinon
              2 : si annuler
*/
int lire_coup(int *x, int tmp_x, int tmp_y){

  assert(NULL != x);

    if(tmp_x >= 25 && tmp_y >= 25 && tmp_y <= ((8 * PIXEL) + 25) && tmp_x <= ((8 * PIXEL) + 25)) {
        tmp_x = (tmp_x - 25) / PIXEL;
        tmp_y = (tmp_y - 25) / PIXEL;
        if(tmp_y < 7) {
            tmp_x += 8 * (8 - tmp_y -1);
        }    
    }
    else if(tmp_x >= 500 && tmp_y >= 600 && tmp_y <= 620 && tmp_x <= 560) {
        return 2;
    }
    else
        return 0;

    *x = tmp_x;

    return 1;
}


int initialise_tab_pos_dames(int tab[]) {
    int i;

    for (i = 0; i < 16 ; ++i) {
        tab[i] = -1;
    }

    return 1;
}



int annuler_coup(Position *pos_reine, Position *pos_cav, MLV_Image *image_reine, MLV_Image *image_cav, int tab_position_dames[], int *cpt_dames) {

    assert(NULL != pos_reine);
    assert(NULL != pos_cav);
    assert(NULL != cpt_dames);
    assert(NULL != image_cav);
    assert(NULL != image_reine);


    /*DEBUG
    int i;
    printf("\ncpt = %d", *cpt_dames);
    for(i = 0; i < 16; ++i){
           printf("[%d] ", tab_position_dames[i]);
    }
    */
    if(tab_position_dames[(*cpt_dames) - 1] != -1){
                /*DEBUG
        printf("\ntab = [%d] i = %d\n", tab_position_dames[*cpt_dames - 1] , *cpt_dames);
        printf("\n");
        */
        

        if(tab_position_dames[(*cpt_dames) - 2] == 0)
            suprimer_dame_position(pos_reine, tab_position_dames[(*cpt_dames) - 1]);
        else if(tab_position_dames[(*cpt_dames) - 2] == 1)
            suprimer_dame_position(pos_cav, tab_position_dames[(*cpt_dames) - 1]);
        tab_position_dames[(*cpt_dames) - 2] = -1;
        tab_position_dames[(*cpt_dames) - 1] = -1;
        (*cpt_dames) -= 2;
        /*printf("\nmodf tab = [%d] i = %d\n", tab_position_dames[*cpt_dames - 1] , *cpt_dames);*/
        MLV_clear_window( MLV_COLOR_BLACK );
        afficher_quadrillage();
        afficher_dame(*pos_reine, image_reine);
        afficher_dame(*pos_cav, image_cav);
        MLV_actualise_window();
    }

    return 1;
    
}








void jouer_graphique(Position pos_reine, Position pos_cav){
    int x;
    MLV_Image *image_reine = NULL;
    MLV_Image *image_cav = NULL;
    Position tmp_pos_reine;
    Position tmp_pos_cav;
    int tab_position_dames[16] = {0};
    int cpt_dames = 0;
    int clic_x, clic_y;

    initialise_tab_pos_dames(tab_position_dames);

    image_reine = MLV_load_image("reine.png");
    image_cav = MLV_load_image("cav.png");

    afficher_quadrillage();


    while(cpt_dames != 16) {

        
        if (MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_PRESSED) {
            MLV_get_mouse_position( &clic_x, &clic_y );
            if (lire_coup(&x, clic_x, clic_y) == 1) {
                if (!est_case_occupee(pos_reine, x) && !est_case_occupee(pos_cav, x)) {
                    tmp_pos_reine = pos_reine;
                    placer_dame_position(&tmp_pos_reine, x);
                    if (est_sans_attaque_mutuelle(tmp_pos_reine, pos_cav)) {
                        placer_dame_position(&pos_reine, x);
                        tab_position_dames[cpt_dames] = 0;
                        tab_position_dames[cpt_dames + 1] = x;
                        cpt_dames += 2;
                        afficher_dame(pos_reine, image_reine);
                    }
                    else{
                        MLV_draw_adapted_text_box(
                        300, 600,
                        "Mauvais coup", 5,
                        MLV_COLOR_RED, MLV_COLOR_GREEN, MLV_COLOR_BLACK,
                        MLV_TEXT_LEFT);
                        MLV_actualise_window();
                        MLV_draw_filled_rectangle(300, 600, 120, 30,MLV_COLOR_BLACK);
                    }
                }
                else{
                    MLV_draw_adapted_text_box(
                    300, 600,
                    "Deja occuper", 5,
                    MLV_COLOR_RED, MLV_COLOR_GREEN, MLV_COLOR_BLACK,
                    MLV_TEXT_LEFT);
                    MLV_actualise_window();
                    MLV_draw_filled_rectangle(300, 600, 120, 30,MLV_COLOR_BLACK);
                }
            }
    
        else if(lire_coup(&x, clic_x, clic_y) == 2 && cpt_dames > 0)
            annuler_coup(&pos_reine, &pos_cav, image_reine, image_cav, tab_position_dames, &cpt_dames);
        MLV_wait_seconds(1);
        }
    
        else if ( MLV_get_mouse_button_state( MLV_BUTTON_RIGHT ) == MLV_PRESSED) {
            MLV_get_mouse_position( &clic_x, &clic_y );
            if(lire_coup(&x, clic_x, clic_y) == 1) {
                if(!est_case_occupee(pos_reine, x) && !est_case_occupee(pos_cav, x)) {
                    tmp_pos_cav = pos_cav;
                    placer_dame_position(&tmp_pos_cav, x);
                    if(est_sans_attaque_mutuelle(pos_reine, tmp_pos_cav)){
                        placer_dame_position(&pos_cav, x);
                        tab_position_dames[cpt_dames] = 1;
                        tab_position_dames[cpt_dames + 1] = x;
                        cpt_dames += 2;
                        afficher_dame(pos_cav,  image_cav);
                    }
                    else{
                        MLV_draw_adapted_text_box(
                        300, 600,
                        "Mauvais coups", 5,
                        MLV_COLOR_RED, MLV_COLOR_GREEN, MLV_COLOR_BLACK, MLV_TEXT_LEFT);
                        MLV_actualise_window();
                        MLV_draw_filled_rectangle(300, 600, 120, 30, MLV_COLOR_BLACK);
                    }
               }
               else{
                    MLV_draw_adapted_text_box(
                    300, 600,
                    "Deja occuper", 5,
                    MLV_COLOR_RED, MLV_COLOR_GREEN, MLV_COLOR_BLACK, MLV_TEXT_LEFT);
                    MLV_actualise_window();
                    MLV_draw_filled_rectangle(300, 600, 120, 30, MLV_COLOR_BLACK);             
                }
            }
    

        else if(lire_coup(&x, clic_x, clic_y) == 2 && cpt_dames > 0)
            annuler_coup(&pos_reine, &pos_cav, image_reine, image_cav, tab_position_dames, &cpt_dames);
    
        MLV_wait_seconds(1);
    }
    

    
    
    MLV_actualise_window();
    }
}




