#include <stdio.h>

/** Test si le bit position de n de type unsigned int long long int est 0 ou 1 */
int testBit_ull(unsigned long long n, int position)
{
    return (n & (1uLL << position)) ? 1 : 0;
}

/** Test si le bit position de n de type char est 0 ou 1 */
int testBit_char(char n, int position)
{
    return (n & (1 << position)) ? 1 : 0;
}

/** affiche la representation binaire sur 64 bits de n */
void afficher_binaire(unsigned long long n)
{
    long long i = 0;

    printf("%llu : ", (unsigned long long)n);
    printf("HEXA %x : ", n);

    for (i = ((8 * sizeof(unsigned long long))-1); i >= 0; i--)
    {
        if ((i+1)%8 == 0)
        {
            printf("|");
        }
        /*
        if (0ull == testBit_ull(n, i))
        {
            printf("0");
        }
        else
        {
            printf("1");
        }
        */
        testBit_ull(n, i) == 0ull ? printf("0") : printf("1");
    }
    printf("\n");
}

void afficher_char(char c)
{
    int i;

    printf("char %c : ", c);
    printf("char/int %d : ", c);

    for (i = 8 * sizeof(char) - 1; i >= 0; i--)
    {
        testBit_char(c, i) ? printf("1") : printf("0");
    }
    printf("\n");
}

int main(){
	unsigned long long x = 1uLL;
    unsigned long long y = 5uLL;
    int i;

/*1*/
    printf("1 : \n");
	afficher_binaire(x<<14);
/*2*/
    printf("2 : \n");
	afficher_binaire(~(x<<13));
/*3*/
    printf("3 : \n");
    afficher_binaire(x<<12);
/*4*/
    printf("4 : \n");
    afficher_binaire(~(x<<13));
/*5*/
    printf("5 : \n");
    afficher_binaire(x);
    printf("%d\n", testBit_ull(x, 10));
/*6*/
    printf("6 : \n");
    x = 7ull;
    afficher_binaire(x);
    printf("%d \n", x & 7ull );
/*7*/
    printf("7 : \n");
    x = 17ull;
    afficher_binaire(x);
    printf("%d \n",!testBit_ull(x, 0) && !testBit_ull(x, 1) && !testBit_ull(x, 2) && !testBit_ull(x, 3) );
/*8*/
    printf("8 : \n");
    afficher_binaire(x);
    afficher_binaire(y);
    printf("%d \n",testBit_ull(x, 10) == testBit_ull(y, 10) );
/*9*/
    printf("9 : \n");
    afficher_binaire(x);
    afficher_binaire(y);
    for (i = 0 ; i < 64 ; ++i){
        if (testBit_ull(x, i) == testBit_ull(y, i)){
            printf("bit a 1 pour les 2 chiffres en position %d \n" );
            break;
        }
    }
/*10*/
    printf("10 : \n");
    afficher_binaire(x);
    afficher_binaire(y);
    for (i = 0 ; i < 64 ; ++i){
        if (!(testBit_ull(x, i) == testBit_ull(y, i))){
            printf("bit different pour les 2 chiffres en position %d \n" );
        }
    }
/*11*/
    printf("11 : \n");
    afficher_binaire(x);
    afficher_binaire(y);
    for (i = 0 ; i < 64 ; ++i){
        if (!(testBit_ull(x, i) != testBit_ull(y, i))){
            printf("bit egaux pour les 2 chiffres en position %d \n" );
        }
    }








    return 0;
}