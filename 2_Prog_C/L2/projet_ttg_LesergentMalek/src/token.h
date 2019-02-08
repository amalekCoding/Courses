#ifndef _H_TOKEN_
#define _H_TOKEN_

#define NOMBRE_TOKENS_EN_HAUT	5

typedef enum
{
	ROUGE = 0,
	VERT,
	BLEU,
	JAUNE
}Couleur;

typedef enum
{
	CERCLE = 0,
	CARRE,
	TRIANGLE,
	DIAMANT
}Forme;

typedef struct token
{
	Couleur couleur;
	Forme forme;
	struct token *suivant;
	struct token *suivant_couleur;
	struct token *precedent_couleur;
	struct token *suivant_forme;
	struct token *precedent_forme;
}Tokens, *Liste;


Tokens *cree_token(Couleur c, Forme f);

void insere_token_debut(Liste *l, Tokens *t);

void insere_token_fin(Liste *l, Tokens *t);

Tokens *libere_token_debut(Liste *l);

Tokens *libere_token_fin(Liste *l);

int nombre_tokens(Liste l);

Tokens *precedent(Liste l);

void decaler_gauche_forme3(Liste l, Tokens *t);

void decaler_gauche_couleur3(Liste l, Tokens *t);

void refait_chainage_couleurs(Liste l);

void liberer_liste(Liste l);

int nombre_formes_consecutives(Liste fin, Liste l);

int nombre_couleurs_consecutives(Liste fin, Liste l);

void refait_chainage_couleurs(Liste l);
void refait_chainage_formes(Liste l);


#endif