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


int addToHeader( csv_t *csv, char *column ){
    // Se o headerNames não tiver nenhum valor, aloca espaço para o primeiro
    if ((!csv->headerNames)){
        csv->headerNames = (char**) malloc(STRING_BUFFER * sizeof(char));
        csv->headerTypes = (char**) malloc(STRING_BUFFER * sizeof(char));

        if (!csv->headerNames || !csv->headerTypes){
            perror("Alocação de memória falhou.");
            return 0;
        }
        return 1;
    }

    // Realoca espaço para o próximo título
    csv->headerNames[csv->columnsCount] = (char*) malloc((strlen(column) + 1) * sizeof(char));
    csv->headerTypes[csv->columnsCount] = (char*) malloc((strlen(column) + 1) * sizeof(char));

    if (!csv->headerNames[csv->columnsCount] || !csv->headerTypes[csv->columnsCount]){
        perror("Alocação de memória falhou.");
        return 0;
    }

    if (isdigit(column[1]))
        strcpy(csv->headerTypes[csv->columnsCount], "[N]");
    else
        strcpy(csv->headerTypes[csv->columnsCount], "[S]"); 

    strcpy(csv->headerNames[csv->columnsCount], column);
    return 1;
}


int readCSV( csv_t *csv ){
    char line[CSV_BUFFER];

    // Cria uma matriz de strings para armazenar o conteúdo do arquivo
    if (!(csv->matrix = (char***) malloc(CSV_BUFFER * sizeof(char)))) return 0; 

    while (fgets(line, sizeof(line), csv->csv_file)){
        char *column, *p = line;
        // Aloca espaço para a linha da matriz
        if (!(csv->matrix[csv->lineCount] = (char **)malloc(STRING_BUFFER * sizeof(char))))
            return 0;

        csv->columnsCount = 0;


        while ((column = strsep(&p, csv->delimiter))){
            // Aloca espaço para o item da matriz
            if (!(csv->matrix[csv->lineCount][csv->columnsCount] = (char *)malloc((strlen(column) + 1) * sizeof(char))))
                return 0;

            // Copia o conteúdo da coluna para o espaço [i, k] da matriz 
            strcpy(csv->matrix[csv->lineCount][csv->columnsCount], column);

            // Adiciona o título da coluna ao header
            if (csv->lineCount == 0) addToHeader(csv, column); 

            if (!column) // Se a coluna for nula, atribui NaN
                column = "NaN";

            (csv->columnsCount)++; // Incrementa o número de colunas
        }
        (csv->lineCount)++; // Incrementa o número de linhas
    }

    return 1;

}