#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <math.h>



#include "case.h"
#include "pomme.h"
#include "serpent.h"
#include "monde.h"
#include "graphique.h"
#include "deuxJoueur.h"
#include "touche_jeu.h"
#include "ia.h"

int deplacerIa(Serpent *sIA, TabJeu *tab_jeu, Pomme *lst_pomme){
    int distance, tmp, x, y, i, indice, sx, sy;
    x = (sIA->pos[0].x - 25) / PIXEL;
    y = (sIA->pos[0].y - 25) / PIXEL;
    sx = sIA->pos[0].x;
    sy = sIA->pos[0].y;
    distance = (int)sqrt((lst_pomme[0].case_pomme.x - x) * (lst_pomme[0].case_pomme.x - x) + (lst_pomme[0].case_pomme.y - y) * (lst_pomme[0].case_pomme.y - y));
    indice = 0;
    for (i = 1 ; i < 3 ; ++i){
        tmp = (int)sqrt((lst_pomme[i].case_pomme.x - x) * (lst_pomme[i].case_pomme.x - x) + (lst_pomme[i].case_pomme.y - y) * (lst_pomme[i].case_pomme.y - y));
        if (tmp < distance){
            distance = tmp;
            indice = i;
        }
    }

    if (x > lst_pomme[indice].case_pomme.x && sIA->dir != E ){
        sIA->pos[0].x = ((sIA->pos[0].x) - PIXEL);
        for(i = sIA->taille; i > 2; --i)
            sIA->pos[i-1] = sIA->pos[i-2];
        sIA->dir = W;
        sIA->pos[1].x = sx;
        sIA->pos[1].y = sy;
        if (tab_jeu->tab[y][x - 1] == 1){
            return manger_pomme_serpent2(sIA, tab_jeu, lst_pomme);
        }
        return 1;
    }
    if (x < lst_pomme[indice].case_pomme.x && sIA->dir != W ){
        sIA->pos[0].x = ((sIA->pos[0].x) + PIXEL);
        for(i = sIA->taille; i > 2; --i)
            sIA->pos[i-1] = sIA->pos[i-2];
        sIA->dir = E;
        sIA->pos[1].x = sx;
        sIA->pos[1].y = sy;
        if (tab_jeu->tab[y][x + 1] == 1){
            return manger_pomme_serpent2(sIA, tab_jeu, lst_pomme);
        }
        return 1;
    }
    if (y > lst_pomme[indice].case_pomme.y && sIA->dir != S ){
        sIA->pos[0].y = ((sIA->pos[0].y) - PIXEL);
        for(i = sIA->taille; i > 2; --i)
            sIA->pos[i-1] = sIA->pos[i-2];
        sIA->dir = No;
        sIA->pos[1].x = sx;
        sIA->pos[1].y = sy;
        if (tab_jeu->tab[y - 1][x] == 1){
            return manger_pomme_serpent2(sIA, tab_jeu, lst_pomme);
        }
        return 1;
    }
    if (y < lst_pomme[indice].case_pomme.y && sIA->dir != No ){
        sIA->pos[0].y = ((sIA->pos[0].y) + PIXEL);
        for(i = sIA->taille; i > 2; --i)
            sIA->pos[i-1] = sIA->pos[i-2];
        sIA->dir = S;
        sIA->pos[1].x = sx;
        sIA->pos[1].y = sy;
        if (tab_jeu->tab[y + 1][x] == 1){
            return manger_pomme_serpent2(sIA, tab_jeu, lst_pomme);
        }
        return 1;
    }
    if (sIA->dir == No || sIA->dir == S){
        sIA->pos[0].x = ((sIA->pos[0].x) + PIXEL);
        for(i = sIA->taille; i > 2; --i)
            sIA->pos[i-1] = sIA->pos[i-2];
        sIA->dir = E;
        sIA->pos[1].x = sx;
        sIA->pos[1].y = sy;
        if (tab_jeu->tab[y][x + 1] == 1){
            return manger_pomme_serpent2(sIA, tab_jeu, lst_pomme);
        }
        return 1;
    }
    if (sIA->dir == E || sIA->dir == W){
        sIA->pos[0].y = ((sIA->pos[0].y) - PIXEL);
        for(i = sIA->taille; i > 2; --i)
            sIA->pos[i-1] = sIA->pos[i-2];
        sIA->dir = No;
        sIA->pos[1].x = sx;
        sIA->pos[1].y = sy;
        if (tab_jeu->tab[y - 1][x] == 1){
            return manger_pomme_serpent2(sIA, tab_jeu, lst_pomme);
        }
        return 1;
    }
    return 0;
}


void jeuIa(Monde *m){
    Serpent sIA;
    int i;
    init_serpent2(&(m->serpent), &(sIA));

    while(1){
        if (!deplacer_serpent2(&(m->serpent),&(m->tab_jeu), m->lst_pomme )){
            printf("IA gagne\n");
            exit(EXIT_FAILURE);
        }

        deplacerIa(&(sIA), &(m->tab_jeu), m->lst_pomme);

                if (!est_valide(sIA)){
            printf("j1 gagne\n");
            exit(EXIT_FAILURE);
        }

        touche_jouer(m, NULL, 1);

        MLV_wait_milliseconds(100);
        afficher_quadrillage();
        afficher_serpent(&(m->serpent));
        afficher_serpent(&(sIA));
        afficher_pomme(m->lst_pomme);
        for(i = 0 ; i < m->serpent.taille ; ++i ){
            if(m->serpent.pos[0].x == sIA.pos[i].x && m->serpent.pos[0].y == sIA.pos[i].y){
                printf("IA gagne\n");
                exit(EXIT_FAILURE);
            }
        }
        for(i = 0 ; i < sIA.taille ; ++i ){
            if(sIA.pos[0].x == m->serpent.pos[i].x && sIA.pos[0].y == m->serpent.pos[i].y){
                printf("Joueur 1 gagne\n");
                exit(EXIT_FAILURE);
            }
        }


        
    }


}