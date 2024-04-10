#ifndef __STRINGITEM__
#define __STRINGITEM__

#define STRING_BUFFER 50

#include <string.h>

/* --------------------------------------- */

/*  Funções de comparação entre strings   */

// a igual a b 
int eq(char* a, char* b);     

// a diferente de b
int neq(char* a, char* b);    

// a maior ou igual a b
int egt(char* a, char* b);   

// a maior que b
int gt(char* a, char* b);     

// a menor ou igual a b
int elt(char* a, char* b);   

//  a menor que b
int lt(char* a, char* b);   

// Compara floats
int cmpFloats(float a, float b);

/* --------------------------------------- */

#endif