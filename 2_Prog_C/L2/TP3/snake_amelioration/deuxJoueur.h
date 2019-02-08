#ifndef __DEUXJOUEUR__
#define __DEUXJOUEUR__ 



typedef struct Monde2{
    Serpent serpent1;
    Serpent serpent2;
    Pomme lst_pomme[NB_POMMES];
    int nb_pomme_manger;
    TabJeu tab_jeu;
}Monde2;

Monde2 init_monde2(int nb_pommes);

void ajouter_pomme_monde2(Monde2 *mon);

int manger_pomme_serpent2(Serpent *serpent, TabJeu *tab_jeu, Pomme *lst_pomme);

void jeu_deux_joueur();

void init_serpent2(Serpent *s1, Serpent *s2);

int deplacer_serpent2(Serpent *serpent, TabJeu *tab_jeu, Pomme *lst_pomme);


#endif