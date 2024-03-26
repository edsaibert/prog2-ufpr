#ifndef __IO__
#define __IO__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define STRING_BUFFER 50
#define CSV_BUFFER 1024
#define DELIMITER ","

char*** countCSV( FILE* csv, int* columnsCount, int* lineCount);

int fileSummary( char*** matrix, int columnsCount, int lineCount);

int showFile( char*** matrix, int columnsCount, int lineCount);

int readFile( char path[STRING_BUFFER], int choice );



#endif 