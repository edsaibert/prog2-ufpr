#ifndef __NULL_H__
#define __NULL_H__

#include "io.h"
#include "filter.h"
#include "stat.h"

/*  opcao 7 */
void switchOption( csv_t* csv );

int filterNan( csv_t* csv, int optionShow, int optionRemove );

void meanSubstitute( char*** matrix, unsigned long int lineCount, unsigned int columnsCount );

void proxSubstitute( char*** matrix, unsigned long int lineCount, unsigned int columnsCount );


#endif