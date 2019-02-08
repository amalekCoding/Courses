#include <stdio.h>
#include <stdlib.h>


#include "token.h"


Tokens *cree_token(Couleur c, Forme f)
{
	Tokens *r = malloc(sizeof(Tokens));
	if(r != NULL)
	{
		r->couleur = c;
		r->forme = f;
		r->suivant = r;
		r->suivant_couleur = r;
		r->precedent_couleur = r;
		r->suivant_forme = r;
		r->precedent_forme = r;
	}
	return r;
}

void insere_token_debut(Liste *l, Tokens *t)
{
	/* La liste est vide, on se contente de la faire pointer sur t */
	if(*l == NULL)
	{
		*l = t;
		if(t != NULL)
		{
			t->suivant = t;
			t->suivant_couleur = t;
			t->suivant_forme = t;
			t->precedent_couleur = t;
			t->precedent_forme = t;
		}
		return;
	}


	/* Liste pointe déjà à la fin. Insère t entre l et l->suivant */
	Liste ancienne_fin = *l;
	Liste debut = (*l)->suivant;

	/* Chaine des tokens */
	ancienne_fin->suivant = t;
	t->suivant = debut;

	/* Parcourt la liste des tokens en mémorisant le dernier token ayant les mêmes attributs. */
	Liste i = t;
	Liste i_forme = t;
	Liste i_couleur = t;

	do
	{
		if(i->couleur == t->couleur)
			i_couleur = i;
		if(i->forme == t->forme)
			i_forme = i;

		i = i->suivant;
	}while(i != t);

	/* Actualise le chainage des précédents. */
	i_couleur->suivant_couleur = t;
	i_forme->suivant_forme = t;
	t->precedent_couleur = i_couleur;
	t->precedent_forme = i_forme;

	/* Fait la même chose dans l'ordre inverse pour le chainage des suivants */
	i = t;
	i_forme = t;
	i_couleur = t;

	do
	{
		if(i->couleur == t->couleur && i_couleur == t)
			i_couleur = i;
		if(i->forme == t->forme && i_forme == t)
			i_forme = i;

		i = i->suivant;
	}while(i != t);

	i_couleur->precedent_couleur = t;
	i_forme->precedent_forme = t;
	t->suivant_couleur = i_couleur;
	t->suivant_forme = i_forme;
}


void insere_token_fin(Liste *l, Tokens *t)
{
	insere_token_debut(l, t);
	/* La nouvelle fin de la liste est le dernier élément ajouté. */
	*l = t;
}

Tokens *libere_token_debut(Liste *l)
{
	if(*l == NULL)
		return NULL;

	/* Un seul élément */
	if((*l)->suivant == *l)
	{
		Tokens *t = *l;
		*l = NULL;
		return t;
	}

	/* Sauvegarde l'ancienne adresse du début */
	Tokens *r = (*l)->suivant;

	/* Saute le premier lien dans le chainage */
	(*l)->suivant = (*l)->suivant->suivant;

	/* Saute également ce token dans les formes et les couleurs. */
	r->precedent_couleur->suivant_couleur = r->suivant_couleur;
	r->precedent_forme->suivant_forme = r->suivant_forme;

	r->suivant_couleur->precedent_couleur = r->precedent_couleur;
	r->suivant_forme->precedent_forme = r->precedent_forme;


	/* Supprime tous les liens avant de retourner le token */
	r->suivant = r;
	r->suivant_couleur = r;
	r->suivant_forme = r;
	r->precedent_couleur = r;
	r->precedent_forme = r;
	return r;
}

Tokens *libere_token_fin(Liste *l)
{
	if(*l == NULL)
		return NULL;

	/* Un seul élément */
	if((*l)->suivant == *l)
	{
		Tokens *t = *l;
		*l = NULL;
		return t;
	}

	/* Sauvegarde l'ancienne adresse de fin */
	Tokens *r = *l;

	/* Fait pointer la liste sur l'élément précédent */
	do
	{
		(*l) = (*l)->suivant;
	}while((*l)->suivant != r);

	/* Saute le premier lien dans le chainage */
	(*l)->suivant = (*l)->suivant->suivant;

	/* Saute également ce token dans les formes et les couleurs. */
	r->precedent_couleur->suivant_couleur = r->suivant_couleur;
	r->precedent_forme->suivant_forme = r->suivant_forme;

	r->suivant_couleur->precedent_couleur = r->precedent_couleur;
	r->suivant_forme->precedent_forme = r->precedent_forme;


	/* Supprime tous les liens avant de retourner le token */
	r->suivant = r;
	r->suivant_couleur = r;
	r->suivant_forme = r;
	r->precedent_couleur = r;
	r->precedent_forme = r;
	return r;
}

int nombre_tokens(Liste l)
{
	if(l == NULL)
		return 0;

	int c = 0;
	l = l->suivant;
	Liste i = l;

	do
	{
		++c;
		i = i->suivant;
	}while(i != l);
	return c;
}

Tokens *precedent(Liste l)
{
	if(l == NULL)
		return NULL;

	Tokens *i = l;
	do
	{
		i = i->suivant;
	}while(i->suivant != l);
	return i;
}

void decaler_gauche_forme3(Liste l, Tokens *t)
{
	if(l == NULL)
		return;

	Couleur tmp_couleur = t->couleur;
	Forme tmp_forme = t->forme;
	Liste tmp = t;

	Liste i = t->suivant_forme;

	do
	{
		t->couleur = i->couleur;
		t->forme = i->forme;
		t = i;
		i = i->suivant_forme;
	}while(i != tmp);
	t->couleur = tmp_couleur;
	t->forme = tmp_forme;
	refait_chainage_couleurs(l);
}

void decaler_gauche_couleur3(Liste l, Tokens *t)
{
	if(l == NULL)
		return;

	Couleur tmp_couleur = t->couleur;
	Forme tmp_forme = t->forme;
	Liste tmp = t;

	Liste i = t->suivant_couleur;

	do
	{
		t->couleur = i->couleur;
		t->forme = i->forme;
		t = i;
		i = i->suivant_couleur;
	}while(i != tmp);
	t->couleur = tmp_couleur;
	t->forme = tmp_forme;
	refait_chainage_formes(l);
}

void refait_chainage_couleurs(Liste l)
{
	/* Refait le chainage des couleurs si celui ci est invalide */
	if(l == NULL)
		return;
	Liste i = l;
	Liste j;
	Liste k;

	do
	{
		j = i;
		k = i;
		do
		{
			if(j->couleur == i->couleur)
			{
				k->suivant_couleur = j;
				j->precedent_couleur = k;
				k = j;
			}
			j = j->suivant;
		}while(j != i);
		i = i->suivant;
	}while(i != l);
}

void refait_chainage_formes(Liste l)
{
	/* Refait le chainage des formes si celui ci est invalide */
	if(l == NULL)
		return;
	Liste i = l;
	Liste j;
	Liste k;

	do
	{
		j = i;
		k = i;
		do
		{
			if(j->forme == i->forme)
			{
				k->suivant_forme = j;
				j->precedent_forme = k;
				k = j;
			}
			j = j->suivant;
		}while(j != i);
		i = i->suivant;
	}while(i != l);
}


void liberer_liste(Liste l)
{
	while(nombre_tokens(l) > 0)
	{
		free(libere_token_fin(&l));
	}
}



int nombre_formes_consecutives(Liste fin, Liste l)
{
	if(fin == NULL || l == NULL)
		return 0;

	Forme f = l->forme;
	int n = 0;
	while(l->forme == f)
	{
		++n;
		if(l == fin)
			break;
		l = l->suivant;
	}
	return n;
}

int nombre_couleurs_consecutives(Liste fin, Liste l)
{
	if(fin == NULL || l == NULL)
		return 0;

	Couleur f = l->couleur;
	int n = 0;
	while(l->couleur == f)
	{
		++n;
		if(l == fin)
			break;
		l = l->suivant;
	}
	return n;
}


void affiche_token_ascii(Tokens *t)
{
	switch(t->forme)
	{
		case CERCLE:
			putchar('(');
			break;
		case CARRE:
			putchar('[');
			break;
		case TRIANGLE:
			putchar('/');
			break;
		case DIAMANT:
			putchar('<');
			break;
		default:
			putchar('?');
			break;
	}
	switch(t->couleur)
	{
		case ROUGE:
			putchar('R');
			break;
		case VERT:
			putchar('V');
			break;
		case BLEU:
			putchar('B');
			break;
		case JAUNE:
			putchar('J');
			break;
		default:
			putchar('?');
			break;
	}
	switch(t->forme)
	{
		case CERCLE:
			putchar(')');
			break;
		case CARRE:
			putchar(']');
			break;
		case TRIANGLE:
			putchar('\\');
			break;
		case DIAMANT:
			putchar('>');
			break;
		default:
			putchar('?');
			break;
	}
}

void affiche_tokens(Liste l)
{
	if(l == NULL)
		return;

	/* Commence du début */
	l = l->suivant;
	Liste i = l;

	printf("\n");
	do
	{
		affiche_token_ascii(i);
		printf(" -> ");
		i = i->suivant;
	}while(i != l);
}





void affiche_tokens_forme(Liste l)
{
	Liste i = l;

	printf("\nFormes similaires:\n");
	do
	{
		affiche_token_ascii(i);
		printf(" -> ");
		i = i->suivant_forme;
	}while(i != l);
}

void affiche_tokens_couleur(Liste l)
{
	Liste i = l;

	printf("\nCouleurs similaires:\n");
	do
	{
		affiche_token_ascii(i);
		printf(" -> ");
		i = i->suivant_couleur;
	}while(i != l);
}


/* ----------- */

void affiche_debug(Liste l)
{
	/* Commence du début */
	affiche_tokens(l);
	l = l->suivant;
	Liste i = l;


	printf("\n");
	do
	{
		printf("\n---\n");
		affiche_token_ascii(i);
		affiche_tokens_forme(i);
		affiche_tokens_couleur(i);
		i = i->suivant;
	}while(i != l);
}


