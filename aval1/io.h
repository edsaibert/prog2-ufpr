#ifndef __IO__
#define __IO__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STRING_BUFFER 50
#define COLUMN_BUFFER 1024

int readFile( char path[STRING_BUFFER] );

int writeFile( FILE* csv );


#endif 