#ifndef __IO__
#define __IO__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*  variáveis globais   */
#define STRING_BUFFER[50]
#define CSV_BUFFER 1024
#define DELIMITER ","


/*  struct do csv   */
typedef struct csv{

    FILE *csv_file;                  /*  Arquivo no qual estão os dados separados por um delimitador */
    char ***matrix;                  /*  Matriz que irá facilitar o uso dos dados no programa    */
    unsigned int fileSize;           /*  Número de bytes do programa */
    unsigned int columnsCount;       /*  Número de colunas da matriz */
    unsigned int lineCount;          /*  Número de linhas da matriz  */
    char delimiter[2];               /*  Delimitador dos dados   */
    char **headerNames;              /*  Títulos da tabela   */
    char **headerTypes;              /*  Tipos de dados da tabela   */
    int  *index;                     /*  Vetor com a coluna index*/

} csv_t;


/* --------------------------------------- */
/* Funções de inicializacao */

/*   inicializeCSV  */
csv_t* inicializeCSV( char* path );

/*  Retorna o tamanho do arquivo com pase em FSEEK e FTELL  */
int fileSize( FILE* csv_file );

/*  Adiciona os títulos do arquivo csv na struct csv_t  */
int addToHeader( csv_t *csv);

/*  Adiciona os indexes de cada linha ao vetor csv->index   */
int addToIndex( csv_t* csv );

/*   Faz a leitura dos valores do csv na matriz e conta o número de linhas e colunas   */
int readCSV( csv_t *csv );
/* --------------------------------------- */

/*  Funções de filtro   */
int eq(char* a, char* b);     // a igual b
int neq(char* a, char* b);    // a diferente de b
int egt(char* a, char* b);    // a maior ou igual a b
int gt(char* a, char* b);     // a maior que b 
int elt(char* a, char* b);   // a menor ou igual a b
int lt(char* a, char* b);    // a menor que b

/* --------------------------------------- */

/*  Funções opcao 1 */

/*  Imprime como uma tabela  */
void formatAsTable( char **f, int* maxStrlen, unsigned int columnsCount, char** string );

/*  Constrói o resumo dos 5 primeiros itens e últimos 5 itens   */
int showFile( char*** matrix, int* index, int lineCount, int columnsCount );

/* --------------------------------------- */

/*  Funcoes opcao 2 */

/*  Diz quais colunas estão no csv  */
void fileSummary( csv_t *csv );

/* --------------------------------------- */

/*  Funcoes opcao 3 */

/*  Filta as linhas com base em alguma comparação   */
int filterFile( csv_t* csv, int index, char* value, int (*func)(char* a, char* b) );

/*  Controla a entrada do usuario */
void filterEntry( csv_t* );

/* --------------------------------------- */

/* Funções de free  */

/*  Libera a memória alocada    */
void freeMatrix( char*** matrix, unsigned int columnsCount, unsigned int lineCount );

/*  Libera a memória do CSV */
void freeCSV( csv_t *csv );

/* Libera a memória do vetor de header  */
void freeHeader( char** types, char** names, unsigned int columnsCount );
/* --------------------------------------- */
#endif 
