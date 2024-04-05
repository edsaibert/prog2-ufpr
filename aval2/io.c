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

    // csv->buffer = (char*) malloc(CSV_BUFFER * sizeof(char));
    // if (!csv->buffer){
    //     perror("Alocação de memória falhou.");
    //     return NULL;
    // }

    csv->headerNames = NULL;
    csv->headerTypes = NULL;
    csv->matrix = NULL;
    strcpy(csv->delimiter, DELIMITER);
    csv->columnsCount = 0;
    csv->lineCount = 0;

   return csv; 

};


int addToHeader( csv_t* csv ){
    // Alocação do vetor
    csv->headerTypes = (char**) malloc(csv->columnsCount * sizeof(char*));
    csv->headerNames = (char**) malloc(csv->columnsCount * sizeof(char*));

    if (!csv->headerTypes || !csv->headerNames){
        perror("Alocação de memória falhou.");
        return 0;
    }

    for (int i = 0; i < csv->columnsCount; i++){
        // Alocação da posição i 
        csv->headerTypes[i] = (char *)malloc(4 * sizeof(char)); //(necessita apenas de 3 bytes para [N] ou [S] + /0)
        csv->headerNames[i] = (char *)malloc(STRING_BUFFER * sizeof(char));

        if (!csv->headerTypes[i] || !csv->headerNames[i]){
            perror("Alocação de memória falhou.");
            return 0;
        }

        // Copia o nome das colunas do header para o vetor headerNames
        strcpy(csv->headerNames[i], csv->matrix[0][i]);
        printf("%s ", csv->headerNames[i]);

        // Copia os tipos das colunas do header para o vetor headerNames
        if (isdigit(csv->matrix[1][i][1])){
            strcpy(csv->headerTypes[i], "[N]");
        } 
        else {
            strcpy(csv->headerTypes[i], "[S]");
        }
    }
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

            if (!column) // Se a coluna for nula, atribui NaN
                strcpy(column, "NaN");

            (csv->columnsCount)++; // Incrementa o número de colunas
        }
        (csv->lineCount)++; // Incrementa o número de linhas
    }

    addToHeader(csv);
    return 1;

}


void formatAsTable( csv_t* csv ){
    int maior = 0;
    int current;

    for (int j = 0; j < csv->columnsCount; j++) {
        for (int i = 0; i < csv->lineCount; i++) {
            current = strlen(csv->matrix[i][j]); 
            if (current > maior)
                maior = current;
        }
    }

    for (int j = 0; j < csv->columnsCount; j++){
        for (int i = 0; i < csv->lineCount; i++){
            current = strlen(csv->matrix[i][j]);
            if (current == maior) continue;
             
            if (!realloc(csv->matrix[i][j], current + (maior-current))){
                perror("Alocação de memória falhou");
                return;
            };
            for (int k = current; k < current + maior-current; k++)
                csv->matrix[i][j][k] = ' ';
        }
    }
}

void showFile( csv_t* csv ){
    formatAsTable(csv);

    for (int i = 0; i < csv->lineCount; i++){
        for (int j = 0; j < csv->columnsCount; j++)
            printf("%s\t", csv->matrix[i][j]);
        printf("\n");
    }
}

void fileSummary( csv_t* csv ){
    for (int i = 0; i < csv->columnsCount; i++){
        printf("\n%-20s", csv->headerNames[i]);
        printf("%-20s", csv->headerTypes[i]);
    }
    printf("\n%d variaveis encontradas", csv->columnsCount);
    printf("\nPrecione ENTER para continuar\n");
    printf("\n");
}

void freeMatrix( char*** matrix ){
    for (int i = 0; i < CSV_BUFFER; i++){
        for (int j = 0; j < STRING_BUFFER; j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
};

void freeHeader( char** types, char** names, int columnsCount ){
    for (int i = 0; i < columnsCount; i++){
        free(types[i]);
        free(names[i]);
    }
    free(types);
    free(names);
}

void freeCSV( csv_t* csv ){
    freeMatrix(csv->matrix);
    freeHeader(csv->headerTypes, csv->headerNames, csv->columnsCount);
    fclose(csv->csv_file);
}