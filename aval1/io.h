#ifndef __IO__
#define __IO__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define STRING_BUFFER 50
#define CSV_BUFFER 1024
#define DELIMITER ","

/*   Verifica se a matriz foi alocada corretamente  */
char*** matrixIsEmpty(char ***matriz);

/*   Faz a leitura dos valores do csv na matriz e conta o número de linhas e colunas   */
char*** countCSV( FILE* csv, int* columnsCount, int* lineCount);

/*  Diz quais colunas estão no csv  */
void fileSummary( char*** matrix, int columnsCount, int lineCount);

/*  Constrói o resumo dos 5 primeiros itens e últimos 5 itens   */
void showFile( char*** matrix, int columnsCount, int lineCount);

/*  Arquivo que abre e fecha o arquivo  */
int readFile( char path[STRING_BUFFER], int choice );

/*  Libera a memória alocada    */
void freeMatrix( char*** matrix, int lineCount, int columnsCount);



#endif 