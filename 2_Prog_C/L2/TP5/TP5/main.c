#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <MLV/MLV_all.h>

#include "jeu.h"
#include "affichage_graphique.h"
#include "jeu_graphique.h"



int main(){

	Position pos = 0uLL;

	

	initialise_tab_cases_attaque();


	/*
	for(i = 0; i < 64 ; ++i){
		afficher_position(tab_cases_attaquees[i]);
	}
	*/


	

	/*calculer_case_attaques(&pos, D5);*/


	afficher_position(pos);



	printf("\n%s\n", est_sans_attaque_mutuelle(pos)? "Bon" : "Mauvais");





	MLV_create_window("Les huits dames", "", 600, 650);

	
	jouer_graphique(pos);
    
    MLV_free_window();


	return 0;
}