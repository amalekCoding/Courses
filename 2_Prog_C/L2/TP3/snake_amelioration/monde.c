#include <stdio.h>
#include <stdlib.h>

#include "case.h"
#include "pomme.h"
#include "serpent.h"
#include "monde.h"



Monde init_monde(int nb_pommes){
    Monde m;
    ini_TabJeu(&(m.tab_jeu));
    ajouter_pomme_monde(&m);
    m.serpent = init_serpent();
    m.nb_pomme_manger = 0;
    return m;
}

int est_valide(Serpent serpent){
    int i = 0;
    if (serpent.pos[0].x < 25 || serpent.pos[0].x > M * PIXEL + 25 || serpent.pos[0].y < 25 || serpent.pos[0].y > N * PIXEL + 25)
        return 0;
    for (i = 1 ; i < serpent.taille ; ++i){
        if(serpent.pos[0].x == serpent.pos[i].x && serpent.pos[0].y == serpent.pos[i].y)
            return 0;
    }
    return 1;
}


void ajouter_pomme_monde(Monde *mon){
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


int deplacer_serpent(Monde *mon){
    int i;
    int x, y;


    x = mon->serpent.pos[0].x;
    y = mon->serpent.pos[0].y;


    if(mon->serpent.dir == E){
        mon->serpent.pos[0].x = ((mon->serpent.pos[0].x) + PIXEL);
        if(!est_valide(mon->serpent) || mon->tab_jeu.tab[((y -25) /PIXEL)][((x- 25) /PIXEL) + 1] == 1){
            if (mon->tab_jeu.tab[((y -25) /PIXEL)][((x- 25) /PIXEL) + 1] == 1){
                for(i = mon->serpent.taille; i > 2; --i)
                    mon->serpent.pos[i-1] = mon->serpent.pos[i-2];

                mon->serpent.pos[1].x = x;
                mon->serpent.pos[1].y = y;
                return manger_pomme_serpent(mon);
            }
            mon->serpent.pos[0].x = ((mon->serpent.pos[0].x) - PIXEL);
            return 0;
        }
    }
    else if(mon->serpent.dir == W){
        mon->serpent.pos[0].x = ((mon->serpent.pos[0].x) - PIXEL);
        if(!est_valide(mon->serpent) || mon->tab_jeu.tab[((y -25) /PIXEL)][((x- 25) /PIXEL) - 1] == 1){
            if (mon->tab_jeu.tab[((y -25) /PIXEL)][((x- 25) /PIXEL) - 1] == 1){
                for(i = mon->serpent.taille; i > 2; --i)
                    mon->serpent.pos[i-1] = mon->serpent.pos[i-2];

                mon->serpent.pos[1].x = x;
                mon->serpent.pos[1].y = y;
                return manger_pomme_serpent(mon);
            }
            mon->serpent.pos[0].x = ((mon->serpent.pos[0].x) + PIXEL);
            return 0;
        }
    }
    else if(mon->serpent.dir == No){
        mon->serpent.pos[0].y = ((mon->serpent.pos[0].y) - PIXEL);
        if(!est_valide(mon->serpent) || mon->tab_jeu.tab[((y -25) /PIXEL) - 1][((x- 25) /PIXEL)] == 1){
            if (mon->tab_jeu.tab[((y -25) /PIXEL) - 1][((x- 25) /PIXEL)] == 1){
                for(i = mon->serpent.taille; i > 2; --i)
                    mon->serpent.pos[i-1] = mon->serpent.pos[i-2];

                mon->serpent.pos[1].x = x;
                mon->serpent.pos[1].y = y;
                return manger_pomme_serpent(mon);
            }
            mon->serpent.pos[0].y = ((mon->serpent.pos[0].y) + PIXEL);
            return 0;
        }
    }
    else if(mon->serpent.dir == S){
        mon->serpent.pos[0].y = ((mon->serpent.pos[0].y) + PIXEL);
        if(!est_valide(mon->serpent) || mon->tab_jeu.tab[((y -25) /PIXEL) + 1][((x- 25) /PIXEL)] == 1){
            if (mon->tab_jeu.tab[((y -25) /PIXEL) + 1][((x- 25) /PIXEL)] == 1){
                for(i = mon->serpent.taille; i > 2; --i)
                    mon->serpent.pos[i-1] = mon->serpent.pos[i-2];

                mon->serpent.pos[1].x = x;
                mon->serpent.pos[1].y = y;
                return manger_pomme_serpent(mon);
            }
            mon->serpent.pos[0].y = ((mon->serpent.pos[0].y) - PIXEL);
            return 0;
        }
    }

    for(i = mon->serpent.taille; i > 2; --i)
        mon->serpent.pos[i-1] = mon->serpent.pos[i-2];

    mon->serpent.pos[1].x = x;
    mon->serpent.pos[1].y = y;

    return 1;
}





/* Initialise le tableau du jeu [N*M] avec des 0 */
void ini_TabJeu(TabJeu * tabj){
  int i, j;
  
  for(i = 0; i < N; ++i){
    for(j = 0; j < M; ++j)
      tabj->tab[i][j] = 0;
}
}

void affiche_tab(TabJeu tab){
    int i, j;

    for(i = 0 ; i < N ; ++i){
        for (j = 0 ; j < M ; ++j){
            printf("%d ",tab.tab[i][j] );
        }
        printf("\n");
    }
}


int manger_pomme_serpent(Monde *mon){
    int i = 0;
    for(i = 0 ; i < 3 ; ++i){
        if ((mon->serpent.pos[0].x - 25) / PIXEL == mon->lst_pomme[i].case_pomme.x && (mon->serpent.pos[0].y - 25) / PIXEL == mon->lst_pomme[i].case_pomme.y){
            mon->tab_jeu.tab[(mon->serpent.pos[0].y - 25) / PIXEL][(mon->serpent.pos[0].x - 25) / PIXEL] = 0;
            mon->lst_pomme[i] = pomme_gen_alea(N, M);
            mon->tab_jeu.tab[mon->lst_pomme[i].case_pomme.y][mon->lst_pomme[i].case_pomme.x] = 1;
            mon->serpent.taille++;
            for(i = mon->serpent.taille; i > 2; --i)
                mon->serpent.pos[i-1] = mon->serpent.pos[i-2];
        }
    }
    return 1;
}