#ifndef __IO__
#define __IO__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define STRING_BUFFER 50
#define CSV_BUFFER 1024
#define DELIMITER ","

typedef struct csv{

    FILE *csv_file;                  /*  Arquivo no qual estão os dados separados por um delimitador */
    char ***matrix;                  /*  Matriz que irá facilitar o uso dos dados no programa    */
    unsigned int fileSize;           /*  Número de bytes do programa */
    unsigned int columnsCount;       /*  Número de colunas da matriz */
    unsigned int lineCount;          /*  Número de linhas da matriz  */
    char delimiter[2];               /*  Delimitador dos dados   */
    char **headerNames;              /*  Títulos da tabela   */
    char **headerTypes;              /*  Tipos de dados da tabela   */

} csv_t;

/*   inicializeCSV  */
csv_t* inicializeCSV( char* path );

/*  Retorna o tamanho do arquivo com pase em FSEEK e FTELL  */
int fileSize( FILE* csv_file );

/*  Imprime como uma tabela  */
void formatAsTable( csv_t* csv );

/*  Adiciona os títulos do arquivo csv na struct csv_t  */
int addToHeader( csv_t *csv);

/*   Faz a leitura dos valores do csv na matriz e conta o número de linhas e colunas   */
int readCSV( csv_t *csv );

/*  Diz quais colunas estão no csv  */
void fileSummary( csv_t *csv );

/*  Constrói o resumo dos 5 primeiros itens e últimos 5 itens   */
void showFile( csv_t *csv );

/*  Arquivo que abre e fecha o arquivo  */
int readFile( csv_t *csv, char path[STRING_BUFFER], int choice );

/*  Libera a memória alocada    */
void freeMatrix( char*** matrix, int fileSize );

/*  Libera a memória do CSV */
void freeCSV( csv_t *csv );

/* Libera a memória do vetor de header  */
void freeHeader( char** types, char** names, unsigned int columnsCount );


#endif 