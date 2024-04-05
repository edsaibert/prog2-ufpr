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

    FILE *csv_file;         /*  Arquivo no qual estão os dados separados por um delimitador */
    char ***matrix;         /*  Matriz que irá facilitar o uso dos dados no programa    */
    int columnsCount;       /*  Número de colunas da matriz */
    int lineCount;          /*  Número de linhas da matriz  */
    char *buffer;           /*  Buffer que irá armazenar temporariamente linhas da matriz   */
    char delimiter[2];      /*  Delimitador dos dados   */
    char **headerNames;     /*  Títulos da tabela   */
    char **headerTypes;     /*  Tipos de dados da tabela   */

} csv_t;

/*   inicializeCSV  */
csv_t* inicializeCSV( char* path );

/*  Imprime como uma tabela  */
void printAsTable( char* column );

/*  Adiciona os títulos do arquivo csv na struct csv_t  */
int addToHeader( csv_t *csv, char *column );

/*  Adiciona os tipos de valores do arquivo csv */
int addToTypes( csv_t *csv );

/*   Faz a leitura dos valores do csv na matriz e conta o número de linhas e colunas   */
int readCSV( csv_t *csv );

/*  Diz quais colunas estão no csv  */
void fileSummary( csv_t *csv );

/*  Constrói o resumo dos 5 primeiros itens e últimos 5 itens   */
void showFile( csv_t *csv );

/*  Arquivo que abre e fecha o arquivo  */
int readFile( csv_t *csv, char path[STRING_BUFFER], int choice );

/*  Libera a memória alocada    */
void freeMatrix( csv_t *csv );

/*  Libera a memória do CSV */
void freeCSV( csv_t *csv );



#endif 