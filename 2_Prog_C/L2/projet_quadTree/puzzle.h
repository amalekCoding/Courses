#define TAILLE_IMAGE	512

typedef struct piece
{
	struct piece *hg, *hd, *bd, *bg;
	int gris;
	int num;
}Piece, *Puzzle;

Piece *allouer_piece(int gris, int num);

int creer_puzzle(const char *nom, Puzzle *p);

void afficher_puzzle(Puzzle p, int coin_x, int coin_y, int taille);