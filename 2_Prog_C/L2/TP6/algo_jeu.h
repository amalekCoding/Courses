#ifndef __ALGO_JEU__
#define __ALGO_JEU__

#define NB_VILLES 1000



typedef struct ville {
    int x;
    int y;
    int est_visite;  /* 0 ou 1 */
}Ville;



typedef struct {
    Ville lst_ville_ordonne[NB_VILLES];             /* Liste villes visiter 1 apres l'une dans l'ordre */
    double distance_ville[NB_VILLES - 1];			/* Distance entre 2 villes de toutes les villes */
    double distance_totale;
}Lst_visite;



int struct_cmp_par_distance(const void *a, const void *b);

double calcul_distance(Ville *v1, Ville *v2);

int modif_visite_mut(Lst_visite *liste_visite, const int alpha, const int nb_villes);

void gene_visite_alea(Lst_visite *liste_visite, Ville *liste_villes, const int nb_alea, const int beta, const int nb_villes);

int rand_a_b(int a, int b);

void algo_jeu(Lst_visite *liste_visite, Lst_visite *liste_visite_copie, Ville *liste_villes, const int nb_visite_alea, const int nb_villes);

Ville* gene_alea_villes(const int nb_villes);


#endif


