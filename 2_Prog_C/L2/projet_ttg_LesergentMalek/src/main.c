#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <MLV/MLV_all.h>
#include "token.h"



#define LARGEUR_FENETRE	800
#define HAUTEUR_FENETRE	800


#define TAILLE_TOKEN	50

#define DUREE_CHRONO	120

int randint(int a, int b)
{
	return rand() % (b - a + 1) + a;
}


int est_dans_zone(int x, int y, int xa, int ya, int w, int h)
{
	if(x >= xa && x <= xa + w)
		return (y >= ya && y <= ya + h);
	return 0;
}

int centrer_liste(Liste l, int taille_token)
{
	/* Taille de la liste en pixels */
	int n = (taille_token + 10) * nombre_tokens(l);
	return (LARGEUR_FENETRE - n) / 2;
}

void dessine_token(Tokens *t, int x, int y, int taille)
{
	MLV_Color c[4];
	c[ROUGE] = MLV_COLOR_RED;
	c[VERT] = MLV_COLOR_GREEN;
	c[BLEU] = MLV_COLOR_BLUE;
	c[JAUNE] = MLV_COLOR_YELLOW;

	/* Cree le polygone en mémoire. px sont les x des sommets et py leurs ordonnées */
	int px[4], py[4];
	int n = 4;

	if(t->forme == CERCLE)
	{
		MLV_draw_filled_circle(x, y, taille / 2, c[t->couleur]);
	}
	else
	{
		switch(t->forme)
		{
			case CARRE:
				px[0] = x - taille / 2;
				px[1] = x + taille / 2;
				px[2] = x + taille / 2;
				px[3] = x - taille / 2;

				py[0] = y - taille / 2;
				py[1] = y - taille / 2;
				py[2] = y + taille / 2;
				py[3] = y + taille / 2;
				break;

			case TRIANGLE:
				px[0] = x;
				px[1] = x + taille / 2;
				px[2] = x - taille / 2;

				py[0] = y - taille / 2;
				py[1] = y + taille / 2;
				py[2] = y + taille / 2;
				n = 3;
				break;

			case DIAMANT:
			default:
				px[0] = x;
				px[1] = x + taille / 2;
				px[2] = x;
				px[3] = x - taille / 2;

				py[0] = y - taille / 2;
				py[1] = y;
				py[2] = y + taille / 2;
				py[3] = y;
				break;

		}
		MLV_draw_filled_polygon(px, py, n, c[t->couleur]);
	}
}

void dessine_liste(Liste l, int x, int y, int taille)
{
	/* x et y du premier jeton! */
	/* Commence du début */
	if(l == NULL)
		return;

	l = l->suivant;
	Liste i = l;

	do
	{
		dessine_token(i, x, y , taille);
		x += taille + 10;
		i = i->suivant;
	}while(i != l);
}

void dessine_menu(int score)
{
	char score_ascii[33];
	sprintf(score_ascii, "%d", score);

	int y = HAUTEUR_FENETRE / 2;
	int x = LARGEUR_FENETRE / 4;

	int w, h;
	MLV_get_size_of_text("<==", &w, &h);


	MLV_draw_filled_rectangle(x - TAILLE_TOKEN, y - TAILLE_TOKEN / 2, 2 * TAILLE_TOKEN, TAILLE_TOKEN, MLV_COLOR_CYAN);

	MLV_draw_text(x - w / 2, y - h / 2, "<==", MLV_COLOR_BLACK);
	--x;
	MLV_draw_text(x - w / 2, y - h / 2, "<==", MLV_COLOR_BLACK);
	--x;
	MLV_draw_text(x - w / 2, y - h / 2, "<==", MLV_COLOR_BLACK);
	--x;
	


	x = 3 * LARGEUR_FENETRE / 4;
	MLV_draw_filled_rectangle(x - TAILLE_TOKEN, y - TAILLE_TOKEN / 2, 2 * TAILLE_TOKEN, TAILLE_TOKEN, MLV_COLOR_CYAN);

	MLV_draw_text(x - w / 2, y - h / 2, "==>", MLV_COLOR_BLACK);
	++x;
	MLV_draw_text(x - w / 2, y - h / 2, "==>", MLV_COLOR_BLACK);
	++x;
	MLV_draw_text(x - w / 2, y - h / 2, "==>", MLV_COLOR_BLACK);
	++x;
	

	MLV_draw_rectangle((LARGEUR_FENETRE - TAILLE_TOKEN - 4) / 2, (HAUTEUR_FENETRE / 3) + (0 - TAILLE_TOKEN - 2) / 2, TAILLE_TOKEN + 5, TAILLE_TOKEN + 5, MLV_COLOR_WHITE);

	MLV_draw_text(0, 0, "Score: ", MLV_COLOR_WHITE);
	MLV_draw_text(60, 0, score_ascii, MLV_COLOR_WHITE);
}


int main(void)
{
	srand(time(NULL));

	int score = 0;
	int x, y;
	int n1, n2;
	int m1, m2;

	Forme forme_tmp;
	Couleur couleur_tmp;

	int taille_token = TAILLE_TOKEN;

	Liste haut = NULL;
	Liste bas = NULL;

	Liste tmp2 = NULL;
	Liste tmp3 = NULL;

	Tokens *t;

	MLV_create_window("Three to go", "icone", LARGEUR_FENETRE, HAUTEUR_FENETRE);

	int continuer = 1;
	

	while(continuer)
	{
		debut:
		/* Remplit la liste du haut jusqu'au nombre de tokens indiqué. */
		while(nombre_tokens(haut) < NOMBRE_TOKENS_EN_HAUT)
		{
			t = cree_token(randint(0, 3), randint(0, 3));
			if(t == NULL)
			{
				continuer = 0;
				break;
			}
			insere_token_debut(&haut, t);
		}
		if(!continuer)
			break;


		/* Test tokens consécutifs à gauche */
		if(bas != NULL)
		{
			t = bas->suivant;
			n1 = nombre_couleurs_consecutives(bas, t);
			n2 = nombre_formes_consecutives(bas, t);
			if(n1 >= 3 && n1 >= n2)
			{
				while(n1 > 0)
				{
					tmp3 = libere_token_debut(&bas);
					free(tmp3);
					--n1;
					++score;
				}
			}
			else if(n2 >= 3 && n2 >= n1)
			{
				while(n2 > 0)
				{
					tmp3 = libere_token_debut(&bas);
					free(tmp3);
					--n2;
					++score;
				}
			}
		}

		/* Test tokens consécutifs à droite */
		if(bas != NULL && bas->suivant != bas)
		{
			m1 = 0;
			m2 = 0;

			t = bas;
			do
			{
				n1 = nombre_couleurs_consecutives(bas, t);
				if(n1 >= m1)
				{
					m1 = n1;
					t = precedent(t);
				}
				else
					break;
			}while(n1 >= m1 && t != bas);

			t = bas;
			do
			{
				n2 = nombre_formes_consecutives(bas, t);
				if(n2 >= m2)
				{
					m2 = n2;
					t = precedent(t);
				}
				else
					break;
			}while(n2 >= m2 && t != bas);

			n1 = m1;
			n2 = m2;

			if(n1 >= 3 && n1 >= n2)
			{
				while(n1 > 0)
				{
					tmp3 = libere_token_fin(&bas);
					free(tmp3);
					--n1;
					++score;
				}
			}
			else if(n2 >= 3 && n2 >= n1)
			{
				while(n2 > 0)
				{
					tmp3 = libere_token_fin(&bas);
					free(tmp3);
					--n2;
					++score;
				}
			}
		}

		/* Change l'échelle des tokens si plus de place */
		if(nombre_tokens(bas) * (taille_token + 10) >= LARGEUR_FENETRE)
		{
			taille_token = (int)(taille_token * 3.0 / 4.0);
		}
		else if(nombre_tokens(bas) * (4.0 * taille_token / 3.0 + 10) <= LARGEUR_FENETRE && (taille_token * 4.0 / 3.0) <= TAILLE_TOKEN)
		{
			taille_token = (int)(taille_token * 4.0 / 3.0);
		}


		/* Affiche les listes et le menu. */
		MLV_clear_window(MLV_COLOR_BLACK);
		dessine_liste(haut, (LARGEUR_FENETRE / 2 - (TAILLE_TOKEN + 10) * (NOMBRE_TOKENS_EN_HAUT - 1)), HAUTEUR_FENETRE / 3, TAILLE_TOKEN);
		dessine_liste(bas, centrer_liste(bas, taille_token), 2 * HAUTEUR_FENETRE / 3, taille_token);
		dessine_menu(score);

		MLV_update_window();

		/* Attend un clic */
		if(MLV_wait_mouse_or_seconds(&x, &y, DUREE_CHRONO) == 1)
		{
			/* L'utilisateur a cliqué a temps */

			if(est_dans_zone(x, y, LARGEUR_FENETRE / 4 - TAILLE_TOKEN, HAUTEUR_FENETRE / 2 - TAILLE_TOKEN / 2, 2 * TAILLE_TOKEN, TAILLE_TOKEN))
			{
				/* Bouton gauche */
				t = libere_token_fin(&haut);
				insere_token_debut(&bas, t);
			}
			if(est_dans_zone(x, y, 3 * LARGEUR_FENETRE / 4 - TAILLE_TOKEN, HAUTEUR_FENETRE / 2 - TAILLE_TOKEN / 2, 2 * TAILLE_TOKEN, TAILLE_TOKEN))
			{
				/* Bouton droit */
				t = libere_token_fin(&haut);
				insere_token_fin(&bas, t);
			}

			/* Clic sur un token? */
			if(bas != NULL)
			{
				tmp2 = bas->suivant;
				tmp3 = tmp2;

				n1 = 0;
				do
				{
					if(est_dans_zone(x, y, centrer_liste(bas, taille_token) + (taille_token + 10) * n1 - taille_token / 2, HAUTEUR_FENETRE * 2 / 3 - taille_token / 2, taille_token, taille_token))
					{
						/* tmp3 est le token sur lequel on a cliqué */
						forme_tmp = tmp3->forme;
						couleur_tmp = tmp3->couleur;

						/* Dessine le menu de décalage */
						/* Meme couleur, forme différente */
						tmp3->forme = (tmp3->forme + 1) % 4;
						dessine_token(tmp3, centrer_liste(bas, taille_token) + (taille_token + 10) * n1, HAUTEUR_FENETRE * 2 / 3 - (taille_token + 10), taille_token);
						tmp3->forme = forme_tmp;

						tmp3->couleur = (tmp3->couleur + 1) % 4;
						dessine_token(tmp3, centrer_liste(bas, taille_token) + (taille_token + 10) * n1, HAUTEUR_FENETRE * 2 / 3 + (taille_token + 10), taille_token);
						tmp3->couleur = couleur_tmp;


						MLV_update_window();

						/* Attend un deuxième clic */
						if(MLV_wait_mouse_or_seconds(&x, &y, DUREE_CHRONO) == 1)
						{
							/* Quel type de decalage faut il faire? */
							if(est_dans_zone(x, y, centrer_liste(bas, taille_token) + (taille_token + 10) * n1 - taille_token / 2, HAUTEUR_FENETRE * 2 / 3 - taille_token / 2 - (taille_token + 10), taille_token, taille_token))
							{
								/* Couleurs */
								decaler_gauche_couleur3(bas, tmp3);
							}
							else if(est_dans_zone(x, y, centrer_liste(bas, taille_token) + (taille_token + 10) * n1 - taille_token / 2, HAUTEUR_FENETRE * 2 / 3 - taille_token / 2 + (taille_token + 10), taille_token, taille_token))
							{
								/* Formes */
								decaler_gauche_forme3(bas, tmp3);
							}
							/* On doit sortir de deux boucles imbriquées, c'est la meilleure solution */
							goto debut;
						}
						else
						{
							/* Trop de temps, fin du jeu */
							continuer = 0;
						}
					}
					tmp3 = tmp3->suivant;
					n1++;
				}while(tmp3 != tmp2);
			}

		}
		else
		{
			/* Trop de temps, fin du jeu */
			continuer = 0;
		}
	}

	MLV_clear_window(MLV_COLOR_BLACK);

	char score_ascii[33];
	sprintf(score_ascii, "%d", score);

	MLV_get_size_of_text("Votre score est :", &x, &y);
	MLV_draw_text((LARGEUR_FENETRE - x)/ 2, HAUTEUR_FENETRE / 2, "Votre score est :", MLV_COLOR_WHITE);

	MLV_get_size_of_text(score_ascii, &x, &y);	
	MLV_draw_text((LARGEUR_FENETRE - x)/ 2, HAUTEUR_FENETRE / 2 + 60, score_ascii, MLV_COLOR_WHITE);

	MLV_update_window();
	MLV_wait_mouse_or_seconds(&x, &y, 10);

	liberer_liste(haut);
	liberer_liste(bas);
	haut = NULL;
	bas = NULL;

	MLV_free_window();
	return 0;
}
