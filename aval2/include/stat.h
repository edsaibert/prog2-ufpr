#ifndef __STAT_H__
#define __STAT_H__

#include "io.h"
#include "sort.h"

/* --------------------------------------- */
/*  Funcoes opcao 4 */

char* trend( char*** matrix, unsigned long int lineCount, unsigned int j, char** unique );

char* mean( char ***matrix, unsigned long int lineCount, unsigned int j );

char* stDeviantion( char ***matrix, unsigned long int lineCount, char* charMean, unsigned int j);

char* median( char*** matrix, unsigned long int lineCount, unsigned int j );

void stat( csv_t* csv );

#endif 