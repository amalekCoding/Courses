#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "case.h"
#include "pomme.h"
#include "serpent.h"
#include "monde.h"
#include "graphique.h"

#define N 32
#define M 64
#define LARGEUR 1200
#define HAUTEUR 700
#define PIXEL ((LARGEUR - 100) / M )

void afficher_quadrillage(){
    int i, j;
    MLV_draw_filled_rectangle(25, 25, M * PIXEL, N * PIXEL, MLV_rgba(187, 93, 54, 250)); 
    for(i = 0; i < N + 1; ++i){
        for(j = 0; j < M + 1; ++j){
            MLV_draw_line(25 + j * PIXEL, 25, 25 + j * PIXEL, N * PIXEL + 25, MLV_COLOR_WHITE);
            MLV_draw_line(25, 25 + i * PIXEL, M * PIXEL + 25, 25 + i * PIXEL, MLV_COLOR_WHITE);
        }
    }
    MLV_actualise_window();

}


void afficher_serpent(Serpent *ser){
    int i;

    MLV_draw_filled_circle(ser->pos[0].x, ser->pos[0].y, PIXEL / 2, MLV_COLOR_RED);
    for(i = 1; i < ser->taille; ++i){
        MLV_draw_filled_circle(ser->pos[i].x, ser->pos[i].y, PIXEL / 2, MLV_COLOR_BLACK);
    }
    MLV_actualise_window();

}

void afficher_pomme(Pomme *pom){
    int i;
    
    for(i = 0; i < 3; ++i){
        MLV_draw_filled_circle(25 + PIXEL / 2 + pom[i].case_pomme.x * PIXEL,25 + PIXEL / 2 + pom[i].case_pomme.y * PIXEL, PIXEL / 2, MLV_COLOR_GREEN);
    }  
    MLV_actualise_window();

}


void afficher_monde(Monde *mon){

    afficher_quadrillage();
    afficher_serpent(&mon->serpent);
    afficher_pomme(mon->lst_pomme);
}


