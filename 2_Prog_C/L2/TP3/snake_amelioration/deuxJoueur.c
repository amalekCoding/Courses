#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>

#include "case.h"
#include "pomme.h"
#include "serpent.h"
#include "monde.h"
#include "graphique.h"
#include "deuxJoueur.h"
#include "touche_jeu.h"

Monde2 init_monde2(int nb_pommes){
    Monde2 m;
    ini_TabJeu(&(m.tab_jeu));
    ajouter_pomme_monde2(&m);
    init_serpent2(&(m.serpent1), &(m.serpent2));
    m.nb_pomme_manger = 0;
    return m;
}

void init_serpent2(Serpent *s1, Serpent *s2){

    int i;
    
    printf("Hello world\n");

    s1->taille = 3;
    s1->dir = E;

    s2->taille = 3;
    s2->dir = W;

    s1->pos[0].x = (M / 4) * PIXEL + 25 + PIXEL / 2;
    s1->pos[0].y = (N / 4) * PIXEL + 25 + PIXEL / 2;

    s2->pos[0].x = 3 * (M / 4) * PIXEL + 25 + PIXEL / 2;
    s2->pos[0].y = 3 * (N / 4) * PIXEL + 25 + PIXEL / 2;

    for(i = 1; i <= 2; ++i){
        s1->pos[i].x = s1->pos[i-1].x - PIXEL;
        s1->pos[i].y = s1->pos[i-1].y;
        s2->pos[i].x = s2->pos[i-1].x + PIXEL;
        s2->pos[i].y = s2->pos[i-1].y;

    }

}

void ajouter_pomme_monde2(Monde2 *mon){
    Pomme pomme_tmp;
    int i; 

    for(i = 0; i < NB_POMMES; ++i){
        do{
            pomme_tmp = pomme_gen_alea(N, M);
        }while(mon->tab_jeu.tab[pomme_tmp.case_pomme.y][pomme_tmp.case_pomme.x]);
        mon->lst_pomme[i] = pomme_tmp;
        mon->tab_jeu.tab[pomme_tmp.case_pomme.y][pomme_tmp.case_pomme.x] = 1;
    }
}




int manger_pomme_serpent2(Serpent *serpent, TabJeu *tab_jeu, Pomme *lst_pomme){
    int i = 0;
    for(i = 0 ; i < 3 ; ++i){
        if ((serpent->pos[0].x - 25) / PIXEL == lst_pomme[i].case_pomme.x && (serpent->pos[0].y - 25) / PIXEL == lst_pomme[i].case_pomme.y){
            tab_jeu->tab[(serpent->pos[0].y - 25) / PIXEL][(serpent->pos[0].x - 25) / PIXEL] = 0;
            lst_pomme[i] = pomme_gen_alea(N, M);
            tab_jeu->tab[lst_pomme[i].case_pomme.y][lst_pomme[i].case_pomme.x] = 1;
            serpent->taille++;
            for(i = serpent->taille; i > 2; --i)
                serpent->pos[i-1] = serpent->pos[i-2];
        }
    }
    return 1;
}


int deplacer_serpent2(Serpent *serpent, TabJeu *tab_jeu, Pomme *lst_pomme){
    int i;
    int x, y;

    x = serpent->pos[0].x;
    y = serpent->pos[0].y;





    if(serpent->dir == E){
        serpent->pos[0].x = ((serpent->pos[0].x) + PIXEL);
        if(!est_valide(*serpent) || tab_jeu->tab[((y -25) /PIXEL)][((x- 25) /PIXEL) + 1] == 1){
            if (tab_jeu->tab[((y -25) /PIXEL)][((x- 25) /PIXEL) + 1] == 1){
                for(i = serpent->taille; i > 2; --i)
                    serpent->pos[i-1] = serpent->pos[i-2];

                serpent->pos[1].x = x;
                serpent->pos[1].y = y;
                return manger_pomme_serpent2(serpent, tab_jeu, lst_pomme);
            }
            serpent->pos[0].x = ((serpent->pos[0].x) - PIXEL);
            return 0;
        }
    }
    else if(serpent->dir == W){
        serpent->pos[0].x = ((serpent->pos[0].x) - PIXEL);
        if(!est_valide(*serpent) || tab_jeu->tab[((y -25) /PIXEL)][((x- 25) /PIXEL) - 1] == 1){
            if (tab_jeu->tab[((y -25) /PIXEL)][((x- 25) /PIXEL) - 1] == 1){
                for(i = serpent->taille; i > 2; --i)
                    serpent->pos[i-1] = serpent->pos[i-2];

                serpent->pos[1].x = x;
                serpent->pos[1].y = y;
                return manger_pomme_serpent2(serpent, tab_jeu, lst_pomme);
            }
            serpent->pos[0].x = ((serpent->pos[0].x) + PIXEL);
            return 0;
        }
    }
    else if(serpent->dir == No){
        serpent->pos[0].y = ((serpent->pos[0].y) - PIXEL);
        if(!est_valide(*serpent) || tab_jeu->tab[((y -25) /PIXEL) - 1][((x- 25) /PIXEL)] == 1){
            if (tab_jeu->tab[((y -25) /PIXEL) - 1][((x- 25) /PIXEL)] == 1){
                for(i = serpent->taille; i > 2; --i)
                    serpent->pos[i-1] = serpent->pos[i-2];

                serpent->pos[1].x = x;
                serpent->pos[1].y = y;
                return manger_pomme_serpent2(serpent, tab_jeu, lst_pomme);
            }
            serpent->pos[0].y = ((serpent->pos[0].y) + PIXEL);
            return 0;
        }
    }
    else if(serpent->dir == S){
        serpent->pos[0].y = ((serpent->pos[0].y) + PIXEL);
        if(!est_valide(*serpent) || tab_jeu->tab[((y -25) /PIXEL) + 1][((x- 25) /PIXEL)] == 1){
            if (tab_jeu->tab[((y -25) /PIXEL) + 1][((x- 25) /PIXEL)] == 1){
                for(i = serpent->taille; i > 2; --i)
                    serpent->pos[i-1] = serpent->pos[i-2];

                serpent->pos[1].x = x;
                serpent->pos[1].y = y;
                return manger_pomme_serpent2(serpent, tab_jeu, lst_pomme);
            }
            serpent->pos[0].y = ((serpent->pos[0].y) - PIXEL);
            return 0;
        }
    }

    for(i = serpent->taille; i > 2; --i)
        serpent->pos[i-1] = serpent->pos[i-2];

    serpent->pos[1].x = x;
    serpent->pos[1].y = y;

    return 1;
}

void jeu_deux_joueur(){
    Monde2 m;
    int i;

    m  = init_monde2(3);
    printf("Le jeu commence\n");
    while(deplacer_serpent2(&(m.serpent1),&(m.tab_jeu), m.lst_pomme ) && deplacer_serpent2(&(m.serpent2),&(m.tab_jeu), m.lst_pomme )){
        for(i = 0 ; i < m.serpent1.taille ; ++i ){
            if(m.serpent1.pos[0].x == m.serpent2.pos[i].x && m.serpent1.pos[0].y == m.serpent2.pos[i].y){
                printf("joueur 2 gagne\n");
                exit(EXIT_FAILURE);
            }
        }
        for(i = 0 ; i < m.serpent2.taille ; ++i ){
            if(m.serpent2.pos[0].x == m.serpent1.pos[i].x && m.serpent2.pos[0].y == m.serpent1.pos[i].y){
                printf("joueur 1 gagne\n");
                exit(EXIT_FAILURE);
            }
        }

        touche_jouer(NULL, &m, 2);

        MLV_wait_milliseconds(100);
        afficher_quadrillage();
        afficher_serpent(&(m.serpent1));
        afficher_serpent(&(m.serpent2));
        afficher_pomme(m.lst_pomme);
    }

}

