#ifndef __JEU_GRAPHIQUE__
#define __JEU_GRAPHIQUE__

#include "jeu.h"


int lire_coup(int *x, int tmp_x, int tmp_y);

int initialise_tab_pos_dames(int tab[]);

void jouer_graphique(Position pos);

int annuler_coup(Position *pos, MLV_Image *image, int tab_position_dames[], int *cpt_dames);





#endif