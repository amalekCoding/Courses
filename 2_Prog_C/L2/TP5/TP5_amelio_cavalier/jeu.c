#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "jeu.h"


Position tab_cases_attaquees[64] = {0uLL};
Position tab_cases_attaquees_cavalier[64] = {0uLL};



int est_case_occupee(Position pos, Case c){

	if(pos & (1uLL << c))
		return 1;

	return 0;
}

int placer_dame_position(Position *pos, Case c){

	if(est_case_occupee(*pos, c))
		return 0;

	*pos = (*pos | (1uLL << c));

	return 1;
}


int suprimer_dame_position(Position *pos, Case c){

	if(est_case_occupee(*pos, c)){
	  *pos = (*pos & (~(1uLL << c)));
		return 1;
	}

	return 0;
}


int afficher_position(Position pos){

	int i, j;

	for(i = 7 ; i >= 0; --i) {
		for(j = 0 ; j < 8 ; ++j){
			printf("%c", est_case_occupee(pos, (8*i) + j)? '+' : '.');
		}
		printf("\n");
	}
	

	return 0;
}

int calculer_case_attaques_cavalier(Position *pos, Case c) {
  
	if(c + 6 < 64)
		*pos = *pos | (1uLL << (c + 6));
	if(c - 6 >= 0)
		*pos = *pos | (1uLL << (c - 6));
	if(c + 10 < 64)
		*pos = *pos | (1uLL << (c + 10));
	if(c - 10 >= 0)
		*pos = *pos | (1uLL << (c - 10));
	if(c + 15 < 64)
		*pos = *pos | (1uLL << (c + 15));
	if(c - 15 >= 0)
		*pos = *pos | (1uLL << (c - 15));
	if(c + 17 < 64)
		*pos = *pos | (1uLL << (c + 17));
	if(c - 17 >= 0)
		*pos = *pos | (1uLL << (c - 17));

  return 1;
}

int calculer_case_attaques_reine(Position *pos, Case c){
	int i, j;


/* Horizontale */
	i = c;
	while(i % 8 != 0){
		--i;
	}
	for(j = 0 ; j < 8; ++j){ 
		if(i == c){
			i += 1;
			continue;
		}
		*pos = *pos | (1ULL << i);
		 i += 1;
	}

/* Verticale */
	i = c;
	while(i > 0){
		if((i - 8) < 0)
			break;
		i -= 8;
	}
	for(j = 0 ; j < 8; ++j){ 
		if(i == c){
			i += 8;
			continue;
		}
		*pos = *pos | (1ULL << i);
		 i += 8;
	}


/* Diagonale droite montante */
	 i = c + 9 ;
	while ( i < 64 && (i % 8 != 0)){ 
		*pos = *pos | (1ULL << i );
		i += 9;
	 }

/* Diagonale droite descendante */
	 i = c - 9 ;
	 while ( i >= 0 && ((i+9) % 8 != 0) ){ 
		 *pos = *pos | (1ULL << i );
		 i -= 9;
	 }

/* Diagonale gauche montante */
	 i = c + 7 ;
	 while ( i < 64 && ((i-7) % 8 != 0)){ 
		 *pos = *pos | (1ULL << i );
		 i += 7;
	 }

/* Diagonale gauche descendante */
	 i = c - 7 ;
	 while ( i > 0 && (i % 8 != 0) ){
		 *pos = *pos | (1ULL << i );
		 i -= 7;
	 }

	 return 1;

}


int initialise_tab_cases_attaque_reine() {
	int i;

	for (i = 0; i < 64 ; ++i)
		calculer_case_attaques_reine(&(tab_cases_attaquees[i]), i);
	


	return 1;
}

int initialise_tab_cases_attaque_cavalier(){
  int i;

  for (i = 0 ; i < 64 ; ++i)
    calculer_case_attaques_cavalier(&(tab_cases_attaquees_cavalier[i]), i);
  
      
  return 1;				   
 }

   
  
int est_sans_attaque_mutuelle(Position pos, Position pos_cavalier) {

	int i;
	Position attaques = 0uLL;

	for(i = 0 ; i < 64 ; ++i){
		if(est_case_occupee(pos, i)){
			attaques |= tab_cases_attaquees[i];
			/*DEBUG
			  printf("\nattaques :\n");
			  afficher_position(attaques);
			*/
		}
		if(est_case_occupee(pos_cavalier, i)){
		    attaques |= tab_cases_attaquees_cavalier[i];
		    printf("\nattaques :\n");
		    afficher_position(attaques);
		}
	}
    /*DEBUG	
	printf("\nPos :\n");
	afficher_position(pos);
    */
	if((pos & attaques) == 0 && (pos_cavalier & attaques) == 0)
		return 1;

	return 0;
}
