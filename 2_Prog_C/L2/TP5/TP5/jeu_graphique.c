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

	for (i = 0; i < 8 ; ++i) {
		tab[i] = -1;
	}

	return 1;
}



int annuler_coup(Position *pos, MLV_Image *image, int tab_position_dames[], int *cpt_dames) {
	int i;


    if(tab_position_dames[*cpt_dames - 1] != -1){

                /*
		printf("\ntab = [%d] i = %d\n", tab_position_dames[*cpt_dames - 1] , *cpt_dames);

		
		for(i = 0; i < 8; ++i){
			printf("[%d] ", tab_position_dames[i]);
		}
		printf("\n");
		*/
		
	    MLV_clear_window( MLV_COLOR_BLACK );
	    afficher_quadrillage();
	    
	    suprimer_dame_position(pos, tab_position_dames[*cpt_dames - 1]);
	    tab_position_dames[*cpt_dames - 1] = -1;
	    (*cpt_dames) -= 1;
	    /*printf("\nmodf tab = [%d] i = %d\n", tab_position_dames[*cpt_dames - 1] , *cpt_dames);*/
	    afficher_dame(*pos, image);
	    MLV_actualise_window();
	}

    return 1;
    
}








void jouer_graphique(Position pos){
    int x;
    MLV_Image *image;
    Position tmp_pos;
    int tab_position_dames[8] = {0};
    int cpt_dames = 0;	

	int clic_x, clic_y;

	initialise_tab_pos_dames(tab_position_dames);

    image = MLV_load_image("tof.png");

	afficher_quadrillage();


	while(cpt_dames != 8) {

	 	MLV_wait_mouse(&clic_x, &clic_y);
		
		
		if(lire_coup(&x, clic_x, clic_y) == 1) {
			if(!est_case_occupee(pos, x)) {
				tmp_pos = pos;
				placer_dame_position(&tmp_pos, x);
				if(est_sans_attaque_mutuelle(tmp_pos)){
					placer_dame_position(&pos, x);
					tab_position_dames[cpt_dames] = x;
					cpt_dames += 1;
					afficher_dame(pos, image);
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
		    annuler_coup(&pos, image,  tab_position_dames, &cpt_dames);
		


		

	
    
    MLV_actualise_window();
  }
}




