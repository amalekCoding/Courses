#include <MLV/MLV_all.h>

#include "case.h"
#include "pomme.h"
#include "serpent.h"
#include "monde.h"
#include "deuxJoueur.h"
#include "touche_jeu.h"



void touche_jouer(Monde *m, Monde2 *m2, int nb_joueur){

        MLV_Keyboard_button touche = MLV_KEYBOARD_NONE;

        if(MLV_get_keyboard_state(MLV_KEYBOARD_SPACE) == MLV_PRESSED){

            while( touche != MLV_KEYBOARD_SPACE ){
                MLV_wait_keyboard(&touche,NULL,NULL);
            }
            touche = MLV_KEYBOARD_NONE;
        }

        if(nb_joueur == 1){
            if( MLV_get_keyboard_state( MLV_KEYBOARD_UP ) == MLV_PRESSED && m->serpent.dir != S){
                m->serpent.dir = No;
            }
            if( MLV_get_keyboard_state( MLV_KEYBOARD_DOWN ) == MLV_PRESSED && m->serpent.dir != No){
                m->serpent.dir = S;
            }
            if( MLV_get_keyboard_state( MLV_KEYBOARD_LEFT ) == MLV_PRESSED && m->serpent.dir != E){
                m->serpent.dir = W;
            }
            if( MLV_get_keyboard_state( MLV_KEYBOARD_RIGHT ) == MLV_PRESSED && m->serpent.dir != W){
                m->serpent.dir = E;
            }
        }

        if(nb_joueur == 2){
            if( MLV_get_keyboard_state( MLV_KEYBOARD_UP ) == MLV_PRESSED && m2->serpent1.dir != S)
                m2->serpent1.dir = No;
            
            if( MLV_get_keyboard_state( MLV_KEYBOARD_DOWN ) == MLV_PRESSED && m2->serpent1.dir != No)
                m2->serpent1.dir = S;
            
            if( MLV_get_keyboard_state( MLV_KEYBOARD_LEFT ) == MLV_PRESSED && m2->serpent1.dir != E)
                m2->serpent1.dir = W;
            
            if( MLV_get_keyboard_state( MLV_KEYBOARD_RIGHT ) == MLV_PRESSED && m2->serpent1.dir != W)
                m2->serpent1.dir = E;


            if( MLV_get_keyboard_state( MLV_KEYBOARD_z ) == MLV_PRESSED && m2->serpent2.dir != S)
                m2->serpent2.dir = No;
            
            if( MLV_get_keyboard_state( MLV_KEYBOARD_s ) == MLV_PRESSED && m2->serpent2.dir != No)
                m2->serpent2.dir = S;
            
            if( MLV_get_keyboard_state( MLV_KEYBOARD_q ) == MLV_PRESSED && m2->serpent2.dir != E)
                m2->serpent2.dir = W;
            
            if( MLV_get_keyboard_state( MLV_KEYBOARD_d ) == MLV_PRESSED && m2->serpent2.dir != W)
                m2->serpent2.dir = E;
        }

}