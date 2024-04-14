#ifndef __SORT_H__
#define __SORT_H__

#include "io.h"

/* --------------------------------------- */
/*  Funcoes opcao 5 */

/*  Desloca os ponteiros    */
void shiftPointers( char ***auxMatrix, long int m, long int i);

/*  Funcao de ordenacao quicksort   */
void quicksort( char ***auxMatrix, long int i, unsigned int j, long int n, int (*cmp)(const void*, const void*));

/*  Ordena as linhas do csv com base em uma coluna   */
void sortFile( csv_t* csv );

#endif