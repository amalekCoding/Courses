#ifndef __CASE__
#define __CASE__

#define N 32
#define M 64
#define NB_POMMES 3
#define LARGEUR 1200
#define HAUTEUR 700
#define PIXEL ((LARGEUR - 100) / M )

typedef struct Case{
    int x;
    int y;
}Case;

void affiche_case(Case c);

#endif