#include <stdio.h>
#include <stdlib.h>
#include "case.h"
#include "serpent.h"




Serpent init_serpent(){
    Serpent s;
    int i;
    
    s.taille = 3;
    s.dir = E;
   
    s.pos[0].x = (M / 2) * PIXEL + 25 + PIXEL / 2;
    s.pos[0].y = (N / 2) * PIXEL + 25 + PIXEL / 2;
    for(i = 1; i <= 2; ++i){
        s.pos[i].x = s.pos[i-1].x - PIXEL;
        s.pos[i].y = s.pos[i-1].y;
    }

    return s;
}