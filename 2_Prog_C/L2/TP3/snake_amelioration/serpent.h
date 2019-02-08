#ifndef __serpent__
#define __serpent__ 




typedef enum{
    No,
    E,
    W,
    S
}Direction;

typedef struct Serpent{
    Case pos[N * M];
    int taille;
    Direction dir;
}Serpent;


Serpent init_serpent();

#endif