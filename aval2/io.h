#ifndef __IO__
#define __IO__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "stringItem.h"     // Funções de comparação de strings
#include "floatItem.h"      //  de comparação de floats

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


/* --------------------------------------- */
/* Funções de inicializacao */

/*  Abre ou cria um arquivo */
FILE* createFile( char* path, char* mode );

/*   inicializeCSV  */
csv_t* inicializeCSV( char* path );

/*  Retorna o tamanho do arquivo com pase em FSEEK e FTELL  */
int fileSize( FILE* csv_file );

/*  Adiciona os tipos do arquivo csv na struct csv_t  */
int addToHeader( csv_t *csv );

/*  Adiciona os indexes de cada linha ao vetor csv->index   */
int addToIndex( csv_t* csv );

/*   Faz a leitura dos valores do csv na matriz e conta o número de linhas e colunas   */
int readCSV( csv_t *csv );


/* --------------------------------------- */

/*  Funções opcao 1 */

/*  Imprime como uma tabela  */
void formatAsTable( char **f, long unsigned int* maxStrlen, unsigned int columnsCount, char** string );

/*  Constrói o resumo dos 5 primeiros itens e últimos 5 itens   */
int showFile( char*** matrix, long int* index, unsigned long int lineCount, unsigned int columnsCount );

/* --------------------------------------- */

/*  Funcoes opcao 2 */

/*  Diz quais colunas estão no csv  */
void fileSummary( csv_t *csv );

/* --------------------------------------- */

/*  Funcoes opcao 3 */

/*  Filta as linhas com base em alguma comparação   */
int filterFile( csv_t* csv, long int index, char* value, int (*func)(char* a, char* b) );

/* Print de uma mensagem seguida de um input do usuario */
char* userInput( char* message, int size );

/* Função que busca uma coluna a ser filtrada. Se encontrada, retorna o indice */
int columnSearch( csv_t* csv, char* column );

/*  Controla a entrada do usuario */
void filterEntry( csv_t* );

/* --------------------------------------- */
/*  Funcoes opcao 4 */

/*  Ordena as linhas do csv com base em uma coluna   */
void sortFile( csv_t* csv );

/* --------------------------------------- */
/*  Funcoes opcao 8 */

void populateFile( char*** matrix, unsigned long int lineCount, unsigned int columnsCount );

/* --------------------------------------- */

/* Funções de free  */

/*  Libera a memória da matriz que não satisfazeram o filtro */
void conditionalFree( csv_t* csv, char*** newMatrix, long int* index, unsigned long int newLineCount );

/*  Libera a memória alocada    */
void freeMatrix( char*** matrix, unsigned int columnsCount, unsigned long int lineCount );

/*  Libera a memória do CSV */
void freeCSV( csv_t *csv );

/* Libera a memória do vetor de header  */
void freeHeader( char** types, unsigned int columnsCount );
/* --------------------------------------- */
#endif 
