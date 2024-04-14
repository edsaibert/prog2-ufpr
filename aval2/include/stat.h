#ifndef __STAT_H__
#define __STAT_H__

#include "io.h"
#include "sort.h"

/* --------------------------------------- */
/*  Funcoes opcao 4 */

char* trend( csv_t* csv, unsigned int j, char** unique );

char* mean( csv_t* csv, unsigned int j );

char* stDeviantion( csv_t* csv, char* charMean, unsigned int j);

char* median( char*** matrix, unsigned long int lineCount, unsigned int j );

void stat( csv_t* csv );

#endif 