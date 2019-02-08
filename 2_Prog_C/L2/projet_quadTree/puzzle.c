#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#include <MLV/MLV_all.h>

#include "puzzle.h"

#define COULEUR_FOND	MLV_COLOR_BLACK
#define TOUCHE_AIDE 	MLV_KEYBOARD_a
#define TOUCHE_SOURCE	MLV_KEYBOARD_s
#define TOUCHE_QUITTER	MLV_KEYBOARD_q

#define EST_DANS_INTERVALLE(x, a, b)	((x) >= (a) && (x) <= (b))
#define RANDINT(a, b)	(rand() % ((b) - (a) + 1) + (a))
#define HAUTEUR_BOUTON	50
#define ESPACE_BOUTON	30

typedef enum
{
	ERREUR = -1,
	PUZZLE_16 = 2,
	PUZZLE_64 = 3,
	QUITTER
}options;

Piece *allouer_piece(int gris, int num)
{
	Piece *p;
	p = malloc(sizeof (Piece));
	if(p != NULL)
	{
		p->gris = gris;
		p->hg = p->hd = p->bd = p->bg = NULL;
		p->num = num;
	}
	return p;
}

/* 0 si erreur, 1 sinon */
int decouper_image(Puzzle *p, MLV_Image *image, int coin_x, int coin_y, int taille, int largeur_image, int hauteur_image, int *num)
{
	assert(taille >= 1);
	assert(image != NULL);
	assert(p != NULL);

	(*p) = allouer_piece(0, (*num)++);
	if((*p) == NULL)
		return 0;

	if(taille == 1)
	{
		/* Feuille */
		int r, g, b;
		if(coin_x >= largeur_image || coin_y >= hauteur_image)
			r = g = b = 255;
		else
			MLV_get_pixel_on_image(image, coin_x, coin_y, &r, &g, &b, NULL);
		(*p)->gris = (r + g + b) / 3;
	}
	else
	{
		/* Noeud interne */
		if(!decouper_image(&((*p)->hg), image, coin_x, coin_y, taille / 2, largeur_image, hauteur_image, num))
			return 0;
		if(!decouper_image(&((*p)->hd), image, coin_x + taille / 2, coin_y, taille / 2, largeur_image, hauteur_image, num))
			return 0;
		if(!decouper_image(&((*p)->bd), image, coin_x + taille / 2, coin_y + taille / 2, taille / 2, largeur_image, hauteur_image, num))
			return 0;
		if(!decouper_image(&((*p)->bg), image, coin_x, coin_y + taille / 2, taille / 2, largeur_image, hauteur_image, num))
			return 0;
	}
	return 1;
}

int creer_puzzle(const char *nom, Puzzle *p)
{
	MLV_Image *i;
	int largeur_image, hauteur_image;
	int num;

	assert(nom != NULL);
	assert(p != NULL);

	num = 1;

	/* Ouvre l'image */
	i = MLV_load_image(nom);

	if(i != NULL)
	{
		MLV_resize_image_with_proportions(i, 512, 512);

		/* Creation de l'arbre. */
		MLV_get_image_size(i, &largeur_image, &hauteur_image);
		if(!decouper_image(p, i, 0, 0, TAILLE_IMAGE, largeur_image, hauteur_image, &num))
		{
			free(i);
			return 0;
		}

		free(i);

		/* Mettre les numéros */

		return 1;
	}

	return 0;
}




void afficher_puzzle(Puzzle p, int coin_x, int coin_y, int taille)
{
	assert(taille >= 1);
	assert(coin_x >= 0);
	assert(coin_y >= 0);

	if(p == NULL)
		return;

	if(taille == 1)
	{
		/* Feuille */
		MLV_draw_pixel(coin_x, coin_y, MLV_rgba(p->gris, p->gris, p->gris, 255));
	}
	else
	{
		/* Noeud interne */
		afficher_puzzle(p->hg, coin_x, coin_y, taille / 2);
		afficher_puzzle(p->hd, coin_x + taille / 2, coin_y, taille / 2);
		afficher_puzzle(p->bd, coin_x + taille / 2, coin_y + taille / 2, taille / 2);
		afficher_puzzle(p->bg, coin_x, coin_y + taille / 2, taille / 2);
	}
}

int sauvegarder_puzzle(Puzzle p, FILE *out)
{
	assert(out != NULL);

	if(p == NULL)
		return 1;
	if(p->hg == NULL && p->hd == NULL && p->bg == NULL && p->bd == NULL)
		/* Feuille, écrire 1 suivi du niveau de gris */
		return fprintf(out, "1 %d ", p->gris) >= 0;

	/* Noeud interne, écrire 0 et appeler récursivement la fonction sur les fils. */
	if(fprintf(out, "0 ") < 0)
		return 0;
	return (sauvegarder_puzzle(p->hg, out) && sauvegarder_puzzle(p->hd, out) && sauvegarder_puzzle(p->bd, out) && sauvegarder_puzzle(p->bg, out));
}


int charger_puzzle2(Puzzle *p, FILE *in, int *num)
{
	int n;

	assert(p != NULL);
	assert(in != NULL);

	/* Lis le type du noeud. */
	if(fscanf(in, "%d ", &n) != 1)
		return 0;

	/* Alloue le noeud */
	*p = allouer_piece(0, (*num)++);
	if(*p == NULL)
		return 0;

	if(n == 1)
	{
		/* Feuille => lire également le niveau de gris. */
		if(fscanf(in, "%d ", &((*p)->gris)) != 1)
			return 0;

		/* Numéro ? */

		return 1;
	}
	/* Noeud interne => appel récursif sur les fils. */
	return (charger_puzzle2(&((*p)->hg), in, num) && charger_puzzle2(&((*p)->hd), in, num) && charger_puzzle2(&((*p)->bd), in, num) && charger_puzzle2(&((*p)->bg), in, num));
}

int charger_puzzle(Puzzle *p, FILE *in)
{
	int num;

	assert(p != NULL);
	assert(in != NULL);

	num = 1;

	return charger_puzzle2(p, in, &num);
}

int copier_puzzle(Puzzle source, Puzzle *dest)
{
	assert(dest != NULL);

	if(source == NULL)
		return 1;

	*dest = allouer_piece(source->gris, source->num);
	if(*dest == NULL)
		return 0;

	return copier_puzzle(source->hg, &((*dest)->hg)) && copier_puzzle(source->hd, &((*dest)->hd)) && copier_puzzle(source->bd, &((*dest)->bd)) && copier_puzzle(source->bg, &((*dest)->bg));
}

/* Renvoie l'adresse de la piece sur laquelle le clic aux coordonnées x, y a été fait, NULL sinon. la taille et la position de l'image doit etre renseignée.
 * niveau_max indique le niveau de récursivité maximal autorisé. Il dépend de la taille des pièces. */
Piece **clic_piece(Puzzle *p, int x, int y, int coin_x, int coin_y, int taille, int niveau_max)
{
	assert(p != NULL);
	assert(coin_x >= 0);
	assert(coin_y >= 0);
	assert(taille >= 0);
	assert(niveau_max >= 0);
	assert(x >= 0);
	assert(y >= 0);

	if(*p == NULL)
		return NULL;
	if(niveau_max == 0)
		return p;

	if(x < coin_x || x > coin_x + taille)
		return NULL;
	if(y < coin_y || y > coin_y + taille)
		return NULL;

	/* C'est un clic valide. On regarde dans quel quart il a été fait et on procède récursivement. */
	if(EST_DANS_INTERVALLE(x, coin_x, coin_x + taille / 2))
	{
		if(EST_DANS_INTERVALLE(y, coin_y, coin_y + taille / 2))
			return clic_piece(&((*p)->hg), x, y, coin_x, coin_y, taille / 2, niveau_max - 1);
		return clic_piece(&((*p)->bg), x, y, coin_x, coin_y + taille / 2, taille / 2, niveau_max - 1);
	}
	else
	{
		if(EST_DANS_INTERVALLE(y, coin_y, coin_y + taille / 2))
			return clic_piece(&((*p)->hd), x, y, coin_x + taille / 2, coin_y, taille / 2, niveau_max - 1);
		return clic_piece(&((*p)->bd), x, y, coin_x + taille / 2, coin_y + taille / 2, taille / 2, niveau_max - 1);
	}
}

void echanger_piece(Piece **p1, Piece **p2)
{
	Piece *tmp;

	assert(p1 != NULL);
	assert(p2 != NULL);

	tmp = *p2;
	*p2 = *p1;
	*p1 = tmp;
}

void melanger_puzzle(Puzzle *p, int nombre_echanges, int niveau_max)
{
	Puzzle *tmp1, *tmp2;
	int fils;
	int i;

	assert(p != NULL);
	assert(nombre_echanges >= 0);
	assert(niveau_max >= 0);

	/* Pour le mélange, on effectue n fois les opérations suivantes:
	 * tmp1 = racine
	 * tmp2 = racine
	 * -Tirer i fois un entier entre 0 et 3, ou i est le niveau maximal de récursivité.
	 * --A chaque nombre tiré, tmp1 = tmp1->hg si i = 0, tmp1->hd = 1, ...
	 * -Idem pour tmp2, avec i nombres tirés.
	 * -Echanger tmp1 et tmp2 */

	while(nombre_echanges--)
	{
		tmp1 = p;
		tmp2 = p;

		for(i = 0; i < niveau_max; ++i)
		{
			if((*tmp1) == NULL)
				return;

			fils = RANDINT(0, 3);
			switch(fils)
			{
				case 0:
					tmp1 = &((*tmp1)->hg);
					break;
				case 1:
					tmp1 = &((*tmp1)->hd);
					break;
				case 2:
					tmp1 = &((*tmp1)->bd);
					break;
				case 3:
				default:
					tmp1 = &((*tmp1)->bg);
					break;
			}
		}

		for(i = 0; i < niveau_max; ++i)
		{
			if((*tmp2) == NULL)
				return;

			fils = RANDINT(0, 3);
			switch(fils)
			{
				case 0:
					tmp2 = &((*tmp2)->hg);
					break;
				case 1:
					tmp2 = &((*tmp2)->hd);
					break;
				case 2:
					tmp2 = &((*tmp2)->bd);
					break;
				case 3:
				default:
					tmp2 = &((*tmp2)->bg);
					break;
			}
		}

		echanger_piece(tmp1, tmp2);
	}
}


void masquer_pieces_differentes(Puzzle source, Puzzle p, int x, int y, int taille)
{
	/* Compare les numéros des pièces de source et de p dans le meme ordre et dessine des carrés de la couleur du fond par dessus
	 * les pieces dont les numéros sont différents. */

	assert(taille >= 0);

	if(source == NULL || p == NULL)
		return;

	if(taille == 0)
		return;

	if(source->num != p->num)
	{
		/* Pas besoin de vérifier les fils d'une pièce différente, car ils sont recouverts par le carré. */
		MLV_draw_filled_rectangle(x, y, taille, taille, COULEUR_FOND);
		return;
	}

	masquer_pieces_differentes(source->hg, p->hg, x, y, taille / 2);
	masquer_pieces_differentes(source->hd, p->hd, x + taille / 2, y, taille / 2);
	masquer_pieces_differentes(source->bd, p->bd, x + taille / 2, y + taille / 2, taille / 2);
	masquer_pieces_differentes(source->bg, p->bg, x, y + taille / 2, taille / 2);
}


/* 1 si source et p sont identiques, 0 sinon */
int puzzle_termine(Puzzle source, Puzzle p)
{
	if(source == NULL && p == NULL)
		return 1;
	if(source == NULL || p == NULL)
		return 0;
	if(source->num != p->num)
		return 0;

	return puzzle_termine(source->hg, p->hg) && puzzle_termine(source->hd, p->hd) && puzzle_termine(source->bd, p->bd) && puzzle_termine(source->bg, p->bg);
}


/* Renvoie -1 en cas d'erreur. Bloque jusqu'à une erreur ou une option valide.
 * En cas de réussite, le numéro de l'option sélectionnée est renvoyé et le contenu des pointeurs est mis à jour. Voir l'énumération anonyme ci-dessus. */



/* Renvoie une valeur définie dans l'énumération ci dessus. */
int menu_principal2(Puzzle *p)
{
	FILE *quad;

	const char boutons[5][32] = {"Charger une image", "Charger un .quad", "16 pièces", "64 pièces", "Quitter"};
	char nom_par_defaut[] = "";
	char *nom_fichier = nom_par_defaut;
	char *nom_quad;

	int x, y;
	int i;

	int fichier_charge = 0;

	int valide;

	while(1)
	{
		/* Dessine le menu */
		MLV_clear_window(COULEUR_FOND);


		for(i = 0; i < 5; ++i)
		{
			if((i == 2 || i == 3) && !fichier_charge)
				continue;
			MLV_draw_filled_rectangle(TAILLE_IMAGE / 2 + ESPACE_BOUTON,
				(TAILLE_IMAGE - 6 * (HAUTEUR_BOUTON + ESPACE_BOUTON)) / 2 + i * (HAUTEUR_BOUTON + ESPACE_BOUTON),
				TAILLE_IMAGE / 2 - 2 * ESPACE_BOUTON,
				HAUTEUR_BOUTON,
				MLV_COLOR_GREY);
			MLV_draw_text(TAILLE_IMAGE / 2 + ESPACE_BOUTON + 10,
				(TAILLE_IMAGE - 6 * (HAUTEUR_BOUTON + ESPACE_BOUTON)) / 2 + i * (HAUTEUR_BOUTON + ESPACE_BOUTON) + 2,
				boutons[i],
				MLV_COLOR_BLACK);
		}



		/* Si le fichier est chargé, affiche la miniature à gauche */
		if(fichier_charge)
		{
					/* Affiche le nom du fichier */
		MLV_draw_text(TAILLE_IMAGE / 2 + ESPACE_BOUTON + 10,
			(TAILLE_IMAGE - 6 * (HAUTEUR_BOUTON + ESPACE_BOUTON)) / 2 + 5 * (HAUTEUR_BOUTON + ESPACE_BOUTON) + 2,
			"Fichier: %s",
			fichier_charge? MLV_COLOR_GREEN : MLV_COLOR_RED, nom_fichier);
			/* Cela n'affiche rien car on s'arrete juste avant les feuilles */
			afficher_puzzle(*p, 0, TAILLE_IMAGE / 4, TAILLE_IMAGE / 2);
		}
		else
			MLV_draw_text(TAILLE_IMAGE / 2 ,
			(TAILLE_IMAGE - 6 * (HAUTEUR_BOUTON + ESPACE_BOUTON)) / 2 + 5 * (HAUTEUR_BOUTON + ESPACE_BOUTON) + 2,
			"Pas de fichier donné ou existant",
			MLV_COLOR_RED, nom_fichier);
	

		MLV_update_window();

		/* Attend un clic. */
		MLV_wait_mouse(&x, &y);

		/* Le clic est il dans une zone intéressante? */
		valide = 0;

		if(!EST_DANS_INTERVALLE(x, TAILLE_IMAGE / 2 + ESPACE_BOUTON, TAILLE_IMAGE - ESPACE_BOUTON))
			continue;

		for(i = 0; i < 5; ++i)
		{
			if(EST_DANS_INTERVALLE(y,
				(TAILLE_IMAGE - 6 * (HAUTEUR_BOUTON + ESPACE_BOUTON)) / 2 + i * (HAUTEUR_BOUTON + ESPACE_BOUTON),
				(TAILLE_IMAGE - 6 * (HAUTEUR_BOUTON + ESPACE_BOUTON)) / 2 + i * (HAUTEUR_BOUTON + ESPACE_BOUTON) + HAUTEUR_BOUTON))
			{
				valide = 1;
				break;
			}
		}

		if(!valide)
			continue;

		/* Le clic est sur un bouton valide. L'interpréter. */
		if(i == 4)
			return QUITTER;

		if((i == 2 || i == 3) && fichier_charge)
			return i;

		if(i == 0)
		{
			/* Demande un nom de fichier. */
			MLV_wait_input_box(TAILLE_IMAGE / 2 + ESPACE_BOUTON,
				(TAILLE_IMAGE - 6 * (HAUTEUR_BOUTON + ESPACE_BOUTON)) / 2 + 0 * (HAUTEUR_BOUTON + ESPACE_BOUTON),
				TAILLE_IMAGE / 2 - 2 * ESPACE_BOUTON,
				HAUTEUR_BOUTON,
				MLV_COLOR_ORANGE,
				MLV_COLOR_BLACK,
				MLV_COLOR_ORANGE,
				"Fichier jpg: ",
				&nom_fichier);

			/* Réinitialise le nom si rien n'a été saisi. */
			if(nom_fichier == NULL)
			{
				nom_fichier = nom_par_defaut;
				continue;
			}

			/* On peut maintenant ouvrir le fichier et le transformer en .quad. */
			if(creer_puzzle(nom_fichier, p))
			{
				nom_quad = malloc(strlen(nom_fichier + 2));
				if(nom_quad != NULL)
				{
					/* Remplace "jpg" par "quad" */
					int i = strlen(nom_fichier) - 3;


					strcpy(nom_quad, nom_fichier);
					strcpy(&nom_quad[i], "quad");

					/* Crée le fichier .quad. */
					quad = fopen(nom_quad, "w");
					if(quad != NULL)
					{
						fichier_charge = 1;
						sauvegarder_puzzle(*p, quad);
						fclose(quad);
					}
					else
					{
						fichier_charge = 0;
					}
					free(nom_quad);
				}
				else
				{
					/* libere_puzzle(*p); */
					free(nom_fichier);
					return ERREUR;
				}
			}
			else
			{
				fichier_charge = 0;
			}
		}

		if(i == 1)
		{
			/* Demande un nom de fichier. */
			MLV_wait_input_box(TAILLE_IMAGE / 2 + ESPACE_BOUTON,
				(TAILLE_IMAGE - 6 * (HAUTEUR_BOUTON + ESPACE_BOUTON)) / 2 + 1 * (HAUTEUR_BOUTON + ESPACE_BOUTON),
				TAILLE_IMAGE / 2 - 2 * ESPACE_BOUTON,
				HAUTEUR_BOUTON,
				MLV_COLOR_ORANGE,
				MLV_COLOR_BLACK,
				MLV_COLOR_ORANGE,
				"Fichier quad: ",
				&nom_fichier);

			/* Réinitialise le nom si rien n'a été saisi. */
			if(nom_fichier == NULL)
			{
				nom_fichier = nom_par_defaut;
				continue;
			}

			quad = fopen(nom_fichier, "r");
			if(quad == NULL)
			{
				fichier_charge = 0;
				continue;
			}

			/* On peut maintenant ouvrir le fichier .quad */
			if(!charger_puzzle(p, quad))
			{
				free(nom_fichier);
				return ERREUR;
			}
			fichier_charge = 1;
		}


	}
}

void afficher_stat(int coups, int coups_oeil, int aides){

        MLV_draw_adapted_text_box(
                10,TAILLE_IMAGE + 20,
                "Coup : %d\nAide : %d\nCoup d'oeil : %d",
                5,
                MLV_COLOR_RED, MLV_COLOR_GREEN, MLV_COLOR_BLACK,
                MLV_TEXT_LEFT,  coups, aides, coups_oeil);


        MLV_draw_adapted_text_box(
                TAILLE_IMAGE - 230, TAILLE_IMAGE + 20,
                "Touches : ",
                5,
                MLV_COLOR_RED, MLV_COLOR_GREEN, MLV_COLOR_BLACK,
                MLV_TEXT_LEFT);

        MLV_draw_adapted_text_box(
                TAILLE_IMAGE - 150, TAILLE_IMAGE + 20,
                "Aide : a\nCoup d'oeil : s\nQuitter : q",
                5,
                MLV_COLOR_RED, MLV_COLOR_GREEN, MLV_COLOR_BLACK,
                MLV_TEXT_LEFT);

}


void affiche_quadrillage(int nombre_pieces){
	int i; 


	if(nombre_pieces == PUZZLE_16){
		nombre_pieces = 16;
		for(i = 0; i <= nombre_pieces/4; ++i){
	    
	      		MLV_draw_line(i * (TAILLE_IMAGE/4), 0, i * (TAILLE_IMAGE/4), TAILLE_IMAGE , MLV_COLOR_RED);
	      		MLV_draw_line(0, i * (TAILLE_IMAGE/4), TAILLE_IMAGE,  i * (TAILLE_IMAGE/4), MLV_COLOR_RED);
  		}
	}
  	else if(nombre_pieces == PUZZLE_64){
		nombre_pieces = 64;
		for(i = 0; i <= nombre_pieces/8; ++i){
	    
	      		MLV_draw_line(i * (TAILLE_IMAGE/8), 0, i * (TAILLE_IMAGE/8), TAILLE_IMAGE , MLV_COLOR_RED);
	      		MLV_draw_line(0, i * (TAILLE_IMAGE/8), TAILLE_IMAGE,  i * (TAILLE_IMAGE/8), MLV_COLOR_RED);
  		}
  	}
  	else
  		return;
}


/* 1 en cas de victoire, 0 en cas d'abandon */
int jouer(Puzzle image, Puzzle p, int *coups, int *coups_oeil, int *aides, int nombre_pieces)
{
	Piece **p1, **p2;
	MLV_Keyboard_button sym;
	int clic_x, clic_y;

	while(!puzzle_termine(image, p))
	{
		/* Gestion des déplacements de pièces */
		do
		{
			p1 = NULL;
			p2 = NULL;

			MLV_clear_window(COULEUR_FOND);
			afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
			affiche_quadrillage(nombre_pieces);
			afficher_stat(*coups, *coups_oeil, *aides);
			MLV_update_window();

			if(MLV_wait_keyboard_or_mouse(&sym, NULL, NULL, &clic_x, &clic_y) == MLV_KEY)
			{
				/* Appui sur une touche => Si c'est une touche utilisée par le programme, provoque l'action associée. */
				switch(sym)
				{
					case TOUCHE_SOURCE:
						(*coups_oeil)++;
						/* Afficher l'image source jusqu'au prochain clic ou 5 secondes. */
						MLV_clear_window(COULEUR_FOND);
						afficher_puzzle(image, 0, 0, TAILLE_IMAGE);
						afficher_stat(*coups, *coups_oeil, *aides);
						MLV_update_window();
						MLV_wait_mouse_or_seconds(NULL, NULL, 5);
						break;

					case TOUCHE_AIDE:
						(*aides)++;
						/* Faire clignoter les carrés mal placés. Pour cela on dessine des carrés de la couleur du fond par dessus par intermittence. */
						masquer_pieces_differentes(image, p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						masquer_pieces_differentes(image, p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						masquer_pieces_differentes(image, p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						masquer_pieces_differentes(image, p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						break;

					case TOUCHE_QUITTER:
						return 0;
						break;

					default:
						break;
				}
				/* Attend de nouveau 2 clics. */
				continue;
			}
			p1 = clic_piece(&p, clic_x, clic_y, 0, 0, TAILLE_IMAGE, nombre_pieces);

			if(MLV_wait_keyboard_or_mouse(&sym, NULL, NULL, &clic_x, &clic_y) == MLV_KEY)
			{
				/* Appui sur une touche => Si c'est une touche utilisée par le programme, provoque l'action associée. */
				switch(sym)
				{
					case TOUCHE_SOURCE:
						(*coups_oeil)++;
						/* Afficher l'image source jusqu'au prochain clic ou 5 secondes. */
						MLV_clear_window(COULEUR_FOND);
						afficher_puzzle(image, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_mouse_or_seconds(NULL, NULL, 5);
						break;

					case TOUCHE_AIDE:
						(*aides)++;
						/* Faire clignoter les carrés mal placés. Pour cela on dessine des carrés de la couleur du fond par dessus par intermittence. */
						masquer_pieces_differentes(image, p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						masquer_pieces_differentes(image, p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						masquer_pieces_differentes(image, p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						masquer_pieces_differentes(image, p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
						MLV_update_window();
						MLV_wait_milliseconds(250);
						break;

					case TOUCHE_QUITTER:
						return 0;
						break;

					default:
						break;
				}
				/* Attend de nouveau 2 clics. */
				continue;
			}
			p2 = clic_piece(&p, clic_x, clic_y, 0, 0, TAILLE_IMAGE, nombre_pieces);
		}while(p1 == NULL || p2 == NULL || p1 == p2);

		/* Echange les deux pointeurs. */
		echanger_piece(p1, p2);
		(*coups)++;

		MLV_clear_window(COULEUR_FOND);
		afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
		affiche_quadrillage(nombre_pieces);
		MLV_update_window();
	}

	return 1;
}


int main(int argc, char **argv)
{
	Puzzle image;
	Puzzle p;


	int nombre_pieces;

	int coups = 0;
	int coups_oeil = 0;
	int aides = 0;

	srand(time(NULL));

	MLV_create_window("Puzzle", NULL, TAILLE_IMAGE, TAILLE_IMAGE+ 100);



	nombre_pieces = menu_principal2(&image);

	if(nombre_pieces == ERREUR || nombre_pieces == QUITTER)
	{
		MLV_free_window();
		return -1;
	}

	#define TEST
	#ifndef TEST
	if(!creer_puzzle("image2.jpg", &image))
	{
		fprintf(stderr, "Erreur lors de l'ouverture de l'image.\n");
		exit(EXIT_FAILURE);
	}

	if((quad = fopen("image2.quad", "w")) == NULL)
	{
		fprintf(stderr, "Erreur lors de la creation du fichier .quad.\n");
		exit(EXIT_FAILURE);
	}
	/* Sauvegarde le puzzle une première fois. */
	if(!sauvegarder_puzzle(image, quad))
	{
		fprintf(stderr, "Erreur lors de l'ecriture dans le fichier .quad.\n");
		exit(EXIT_FAILURE);
	}
	/* Ferme le fichier. */
	fclose(quad);
	#endif

	/* Copie l'image dans un second arbre qui pourra etre modifié. */

	if(!copier_puzzle(image, &p))
	{
		fprintf(stderr, "Erreur lors de la copie de l'arbre image.\n");
		exit(EXIT_FAILURE);
	}

	/* Mélanger les pieces. */
	melanger_puzzle(&p, 100, nombre_pieces);

	MLV_clear_window(COULEUR_FOND);
	afficher_puzzle(p, 0, 0, TAILLE_IMAGE);
	MLV_update_window();

	if(jouer(image, p, &coups, &coups_oeil, &aides, nombre_pieces))
	{
		/* Puzzle réussi */


	}
	else
	{
		/* Abandonné */


	}

	MLV_free_window();

	return 0;
}
