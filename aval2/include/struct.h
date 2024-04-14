#ifndef __STRUCT__
#define __STRUCT__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h> 

// Funções de comparação entre strings 
#include "stringItem.h"

#define STRING_BUFFER 50
#define CSV_BUFFER 1024
#define DELIMITER ","


/*  struct do csv   */
typedef struct csv{
    FILE *csv_file;                  /*  Arquivo no qual estão os dados separados por um delimitador */
    char ***matrix;                  /*  Matriz que irá facilitar o uso dos dados no programa    */
    unsigned long int fileSize;      /*  Número de bytes do programa */
    unsigned int columnsCount;       /*  Número de colunas da matriz */
    unsigned long int lineCount;     /*  Número de linhas da matriz  */
    char delimiter[2];               /*  Delimitador dos dados   */
    int *headerTypes;                /*  Tipos de dados da tabela   */
    long int  *index;                /*  Vetor com a coluna index*/

} csv_t;

/*  struct para funcao stat */
typedef struct hash{
    char* item;
    int count;
} hash_t;


#endif