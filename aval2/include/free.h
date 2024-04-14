#ifndef __FREE_H__
#define __FREE_H__

#include "io.h"

/* --------------------------------------- */
/* Funções de free  */

/*  Libera a memória alocada    */
void freeMatrix( char*** matrix, unsigned int columnsCount, unsigned long int lineCount );

/*  Libera a memória do CSV */
void freeCSV( csv_t *csv );

/* --------------------------------------- */

#endif