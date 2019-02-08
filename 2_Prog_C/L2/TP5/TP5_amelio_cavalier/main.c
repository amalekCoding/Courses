#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <MLV/MLV_all.h>

#include "jeu.h"
#include "affichage_graphique.h"
#include "jeu_graphique.h"



int main(){

    Position pos_reine = 0uLL;
    Position pos_cav = 0uLL;

	initialise_tab_cases_attaque_reine();
	initialise_tab_cases_attaque_cavalier();

	int i;
	for(i = 0; i < 64 ; ++i){
        afficher_position(tab_cases_attaquees[i]);
        printf("\n");
	}
	
	afficher_position(pos_reine);




	MLV_create_window("Les huits dames", "", 600, 650);

	
	jouer_graphique(pos_reine, pos_cav);
    
    MLV_free_window();


	return 0;
}
