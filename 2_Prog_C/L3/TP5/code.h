#ifndef __CODE__SEC__
#define __CODE__SEC__

#include <stdint.h>

#define NB_OCT 8

/* System free C type for binary keys */
typedef struct{
  uint8_t values[NB_OCT];
}Bitkey;

/* Return a mesure of the quality of the given key `k`. However, the
   returned value is unsafe. The only warranty is that a perfect key
   has for fitness 100.0000 */
float fitness_key(Bitkey* k);

/* This function allows to enter the matrix only if you have the
   perfect key. Are you ready for this experience ? */
void enter_the_matrix(Bitkey* k);

#endif
