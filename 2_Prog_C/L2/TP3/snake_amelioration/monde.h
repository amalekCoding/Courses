#ifndef __MONDE__
#define __MONDE__ 



typedef struct TabJeu{
    int tab[N][M];
}TabJeu;

typedef struct Monde{
    Serpent serpent;
    Pomme lst_pomme[NB_POMMES];
    int nb_pomme_manger;
    TabJeu tab_jeu;
}Monde;





Monde init_monde(int nb_pommes);

void ajouter_pomme_monde(Monde *mon);

int deplacer_serpent(Monde *mon);

void ini_TabJeu(TabJeu * tabj);

void affiche_tab(TabJeu tab);

int est_valide(Serpent serpent);

int manger_pomme_serpent(Monde *mon);

#endif