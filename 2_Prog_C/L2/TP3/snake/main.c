#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <MLV/MLV_all.h>

#include "case.h"
#include "pomme.h"
#include "serpent.h"
#include "monde.h"
#include "graphique.h"


 


int main(){

    Monde m;
    MLV_Keyboard_button touche = MLV_KEYBOARD_NONE;
    srand(time(NULL));
    m = init_monde(3);

    MLV_create_window("Snake", "", LARGEUR, HAUTEUR);
    afficher_monde(&m);
    MLV_actualise_window();
    MLV_wait_keyboard(NULL, NULL, NULL);

    while(1){

        afficher_monde(&m);


        if( MLV_get_keyboard_state( MLV_KEYBOARD_SPACE) == MLV_PRESSED ){

            while( touche != MLV_KEYBOARD_SPACE ){
                MLV_wait_keyboard(&touche,NULL,NULL);
            }
            touche = MLV_KEYBOARD_NONE;
        }

        if( MLV_get_keyboard_state( MLV_KEYBOARD_UP ) == MLV_PRESSED && m.serpent.dir != S){
            m.serpent.dir = No;
        }
        if( MLV_get_keyboard_state( MLV_KEYBOARD_DOWN ) == MLV_PRESSED && m.serpent.dir != No){
            m.serpent.dir = S;
        }
        if( MLV_get_keyboard_state( MLV_KEYBOARD_LEFT ) == MLV_PRESSED && m.serpent.dir != E){
            m.serpent.dir = W;
        }
        if( MLV_get_keyboard_state( MLV_KEYBOARD_RIGHT ) == MLV_PRESSED && m.serpent.dir != W){
            m.serpent.dir = E;
        }
        MLV_wait_milliseconds(100);
        if (!deplacer_serpent(&m))
            exit(EXIT_FAILURE);
        
        MLV_clear_window( MLV_COLOR_BLACK );

    }

    MLV_free_window();

    return 0;
}