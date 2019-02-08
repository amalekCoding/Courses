#include <stdio.h>
#include <stdlib.h>

#include "case.h"
#include "pomme.h"



int randint(int min, int max){
    return rand() % (max - min + 1) + min;
}
Pomme pomme_gen_alea(int n, int m){
    Pomme pomme;
    
    pomme.case_pomme.x = randint(0, m - 1);
    pomme.case_pomme.y = randint(0, n - 1);

    return pomme;
}