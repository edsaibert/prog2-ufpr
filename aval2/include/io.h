#ifndef __IO__
#define __IO__

#include "struct.h"
/* --------------------------------------- */

/*  Abre ou cria um arquivo */
FILE* createFile( char* path, char* mode );

/*   inicializeCSV  */
csv_t* inicializeCSV( char* path );

/*  Retorna o tamanho do arquivo com pase em FSEEK e FTELL  */
int fileSize( FILE* csv_file );

/* Print de uma mensagem seguida de um input do usuario */
char* userInput( char* message, int size );

/*  Adiciona os tipos do arquivo csv na struct csv_t  */
int addToHeader( csv_t *csv );

/*  Adiciona os indexes de cada linha ao vetor csv->index   */
int addToIndex( csv_t* csv );

/*   Faz a leitura dos valores do csv na matriz e conta o número de linhas e colunas   */
int readCSV( csv_t *csv );

/*  Popula um arquivo   */
void populateFile( char*** matrix, unsigned long int lineCount, unsigned int columnsCount );

/* --------------------------------------- */

/*  Imprime como uma tabela  */
void formatAsTable( char **f, long unsigned int* maxStrlen, unsigned int columnsCount, char** string );

/*  Constrói o resumo dos 5 primeiros itens e últimos 5 itens   */
int showFile( char*** matrix, long int* index, unsigned long int lineCount, unsigned int columnsCount );

/* Função que busca uma coluna a ser filtrada. Se encontrada, retorna o indice */
int columnSearch( csv_t* csv, char* column );

/*  Diz quais colunas estão no csv  */
void fileSummary( csv_t *csv );

#endif 
