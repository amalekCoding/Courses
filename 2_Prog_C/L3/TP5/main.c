#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "code.h"

#define NB_KEY 8


void setBit_pos(uint8_t *nb, const int position) {
    assert(NULL != nb);

	*nb = (*nb | ((uint8_t)1 << position));
}

void unsetBit_pos(uint8_t *nb, const int position) {
    assert(NULL != nb);

	*nb = (*nb & (~((uint8_t)1 << position)));
}

int getBit_pos(const uint8_t nb, const int position) {
    return (nb & ( (uint8_t)1 << position) ) >> position;
}



void generate_cle(Bitkey *b_key) {
    int i, j;

    assert(NULL != b_key);

    for (i = 0; i < NB_OCT; i++)
        b_key->values[i] = (uint8_t)0;

    for (i = 0; i < NB_OCT; i++) {
        for (j = 0; j < 8; j++) {
            if ( rand() % 2 )
				setBit_pos(&(b_key->values[i]), j);
        }
    }
}

int compare_key(const void *key_1, const void *key_2) {
    float key1, key2;

	assert(NULL != key_1);
    assert(NULL != key_2);

    key1 = fitness_key((Bitkey *)key_1);
    key2 = fitness_key((Bitkey *)key_2);
    return (key2 - key1);
}




void three_coupling(Bitkey *new_key, const Bitkey key_1, const Bitkey key_2, const Bitkey key_3) {
    int i, j;
    int kid = 0;

    assert(NULL != new_key);


    for (i = 0; i < 8; i++)
        new_key->values[i] = (uint8_t)0;

    for (i = 0; i < NB_OCT; i++) {
        for (j = 0; j < 8; j++) {
            kid = getBit_pos(key_1.values[i], j) + 
            	  getBit_pos(key_2.values[i], j) + 
            	  getBit_pos(key_3.values[i], j);
            
            if (kid >= 2)
                setBit_pos(&(new_key->values[i]), j);

        }
    }
}



void generate_cle_generation(Bitkey *bit_key, const int x) {
    int i;
    Bitkey *lst_key = NULL;

    assert(NULL != bit_key);

    if (x == 0) {
        generate_cle(bit_key);
        return ;
    }

	lst_key = (Bitkey *) malloc(NB_KEY*sizeof(Bitkey));
    if (NULL == lst_key) {
    	fprintf(stderr, "Error : malloc()\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < NB_KEY; i++) {
        generate_cle_generation(&(lst_key[i]), x-1);
    }

    qsort( &(lst_key[0]), NB_KEY, sizeof(Bitkey), compare_key);
    three_coupling(bit_key, lst_key[0], lst_key[1], lst_key[2]);
    printf("-> Key quality value: %f\n", fitness_key(bit_key));


    free(lst_key);
}





int main() {

	Bitkey cle;
	srand(time(NULL));

	generate_cle_generation(&cle, 7);

	enter_the_matrix(&cle);

	return 0;
}