#ifndef __JEU_GRAPHIQUE__
#define __JEU_GRAPHIQUE__

#include "jeu.h"


int lire_coup(int *x, int tmp_x, int tmp_y);

int initialise_tab_pos_dames(int tab[]);

void jouer_graphique(Position pos, Position pos_cav);

int annuler_coup(Position *pos_reine, Position *pos_cav, MLV_Image *image_reine, MLV_Image *image_cav, int tab_position_dames[], int *cpt_dames);





#endif
