#include "io.h"

csv_t* inicializeCSV( char* path ){

    csv_t* csv = (csv_t*) malloc(sizeof(csv_t));
    if (!csv){
        perror("Alocação de memória falhou.");
        return NULL;
    }

    csv->csv_file = fopen(path, "r");
    if (!csv->csv_file){
        perror("Erro ao abrir o arquivo.");
        return NULL;
    }

    csv->buffer = (char*) malloc(CSV_BUFFER * sizeof(char));
    if (!csv->buffer){
        perror("Alocação de memória falhou.");
        return NULL;
    }

    csv->headerNames = NULL;
    csv->headerTypes = NULL;
    csv->matrix = NULL;
    strcpy(csv->delimiter, DELIMITER);
    csv->columnsCount = 0;
    csv->lineCount = 0;

   return csv; 

};


int readCSV( csv_t *csv ){
    char line[CSV_BUFFER];

    // Cria uma matriz de strings para armazenar o conteúdo do arquivo
    if (!(csv->matrix = (char***) malloc(CSV_BUFFER * sizeof(char)))) return 0; 

    while (fgets(line, sizeof(line), csv->csv_file))
    {
        char *column, *p = line;
        // Aloca espaço para a linha da matriz
        if (!(csv->matrix[csv->lineCount] = (char **)malloc(STRING_BUFFER * sizeof(char))))
            return 0;

        csv->columnsCount = 0;

        while ((column = strsep(&p, csv->delimiter)))
        {
            // Aloca espaço para o item da matriz
            if (!(csv->matrix[csv->lineCount][csv->columnsCount] = (char *)malloc((strlen(column) + 1) * sizeof(char))))
                return 0;

            // Copia o conteúdo da coluna para o espaço [i, k] da matriz 
            strcpy(csv->matrix[csv->lineCount][csv->columnsCount], column);

            if (!column) // Se a coluna for nula, atribui NaN
                column = "NaN";

            (csv->columnsCount)++; // Incrementa o número de colunas
        }
        (csv->lineCount)++; // Incrementa o número de linhas
    }

    return 1;

}