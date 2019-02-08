#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <MLV/MLV_all.h>
#include "affichage_graphique.h"


#define LARGEUR 600
#define PIXEL ((LARGEUR - 50) / 8)




void afficher_dame(Position pos, MLV_Image* image){
    int i, k;
    int cpt_x = 0;
    int cpt_y = 0;
    
	assert(image != NULL);



    for(k = 0 ; k < 64; ++k){
        if(est_case_occupee(pos, k)){
	        i = k;
			while(i % 8 != 0){
				++cpt_x;
				--i;
			}

		    i = k;
			while(i % 8 != 0 && i < 64){
				++cpt_y;
				i += 8;
			}

			i = k;
			while(i % 8 == 0 && i < 64){
				++cpt_y;
				i += 8;
			}
        	MLV_draw_image(image, (cpt_x)* PIXEL + 25, (cpt_y - 1) * PIXEL + 25);
        	cpt_x = 0;
        	cpt_y = 0;

        }

    }
    MLV_actualise_window();

}



void afficher_quadrillage(){
    int i, j;


    MLV_draw_filled_rectangle(25, 25, 8 * PIXEL, 8 * PIXEL, MLV_rgba(187, 93, 54, 250)); 
    for(i = 0; i < 8 + 1; ++i){
        for(j = 0; j < 8 + 1; ++j){
            MLV_draw_line(25 + j * PIXEL, 25, 25 + j * PIXEL, 8 * PIXEL + 25, MLV_COLOR_WHITE);
            MLV_draw_line(25, 25 + i * PIXEL, 8 * PIXEL + 25, 25 + i * PIXEL, MLV_COLOR_WHITE);
        }
    }

    MLV_actualise_window();



            MLV_draw_adapted_text_box(
                500, 600,
                "Annuler",
                5,
                MLV_COLOR_RED, MLV_COLOR_GREEN, MLV_COLOR_BLACK,
                MLV_TEXT_LEFT);




}








