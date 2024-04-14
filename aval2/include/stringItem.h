#ifndef __STRINGITEM__
#define __STRINGITEM__

#include <stdlib.h>
#include <string.h>

/* --------------------------------------- */
/*  Funções de comparação entre strings   */

/* Compara dois valores */
int cmp(const void* a, const void* b);

/* Compara dois valores para o qsort */
int qs_cmp(const void* a, const void* b);

/* Compara dois valores para o qsort (inverso) */
int qs_cmp_i(const void* a, const void* b);

/* Compara se dois valores são iguais */
int eq(const void* a, const void* b);

/* Compara se dois valores são diferentes */
int neq(const void* a, const void* b);

/* Compara se um valor é maior ou igual ao outro */
int egt(const void* a, const void* b);

/* Compara se um valor é maior que o outro */
int gt(const void* a, const void* b);

/* Compara se um valor é menor ou igual ao outro */
int elt(const void* a, const void* b);

/* Compara se um valor é menor que o outro */
int lt(const void* a, const void* b);

/* --------------------------------------- */

#endif

