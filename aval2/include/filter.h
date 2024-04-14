#ifndef __FILTER_H__
#define __FILTER_H__
/*  Funcoes de filtro e de select   */

#include "io.h"

/* --------------------------------------- */
/*  Funcoes opcao 3 */

// Free somente nas linhas que não me interessam (que não estam em index)
void conditionalFree( csv_t* csv, char*** newMatrix, long int* index, unsigned long int newLineCount );

/*  Filta as linhas com base em alguma comparação   */
int filterFile( csv_t* csv, long int index, char* value, int (*func)(const void* a, const void* b) );

/*  Controla a entrada do usuario */
void filterEntry( csv_t* );

/* --------------------------------------- */
/*  Funcoes opcao 6 */


#endif