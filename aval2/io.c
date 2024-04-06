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

    for (unsigned int i = 0; i < csv->columnsCount; i++){
        // Alocação da posição i 
        csv->headerTypes[i] = (char *)malloc(4 * sizeof(char)); //(necessita apenas de 3 bytes para [N] ou [S] + /0)
        csv->headerNames[i] = (char *)malloc(STRING_BUFFER * sizeof(char));

        if (!csv->headerTypes[i] || !csv->headerNames[i]){
            perror("Alocação de memória falhou.");
            return 0;
        }

        // Copia o nome das colunas do header para o vetor headerNames
        strcpy(csv->headerNames[i], csv->matrix[0][i]);

        // Gambiarra se o primeiro valor é NaN
        int j = 1;
        while (strcmp((csv->matrix[j][i]), "") == 0){
            j++;
        }

        // Copia os tipos das colunas do header para o vetor headerNames
        if (isdigit(csv->matrix[j][i][1])){
            strcpy(csv->headerTypes[i], "[N]");
        } 
        else {
            strcpy(csv->headerTypes[i], "[S]");
        }
    }
    return 1;
}

int fileSize( FILE* csv_file ){

    if (csv_file == NULL) return 0;
    /*  Desloca ponteiro para fim do arquivo    */
    fseek(csv_file, 0, SEEK_END);
    /*  Descobre o valor do ponteiro    */
    unsigned long int p = ftell(csv_file);
    /*  Desloca o ponteiro novamente para o começo do arquivo   */
    fseek(csv_file, 0, SEEK_SET);

    return p;
}


int readCSV( csv_t *csv ){
    char line[CSV_BUFFER];
    unsigned long int size = fileSize(csv->csv_file);
    csv->fileSize = size;

    // Cria uma matriz de strings para armazenar o conteúdo do arquivo
    if (!(csv->matrix = (char***) malloc(size * sizeof(char)))) return 0; 

    while (fgets(line, sizeof(line), csv->csv_file)){
        char *column, *p = line;
        // Aloca espaço para a linha da matriz
        if (!(csv->matrix[csv->lineCount] = (char **)malloc(STRING_BUFFER * sizeof(char**))))
            return 0;

        csv->columnsCount = 0;

        while ((column = strsep(&p, csv->delimiter))){

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

    addToHeader(csv);
    return 1;

}

void formatAsTable( csv_t* csv ){
    int current;
    int maior;

    for (unsigned int j = 0; j < csv->columnsCount; j++) {
        maior = 0;
        for (unsigned int i = 0; i < csv->lineCount; i++) {
            current = strlen(csv->matrix[i][j]); 
            if (current > maior)
                maior = current;
        }

        for (unsigned int k = 0; k < csv->lineCount; k++){
                current = strlen(csv->matrix[k][j]);
                if (current == maior)
                    continue;

                if (!realloc(csv->matrix[k][j], current + (maior - current))){
                    perror("Alocação de memória falhou");
                    return;
                };
                for (unsigned int l = current; l < current + maior - current; l++)
                    csv->matrix[k][j][l] = ' ';
    }

    }
}

void showFile( csv_t* csv ){
    formatAsTable(csv);

    for (unsigned int i = 0; i < csv->lineCount; i++){
        for (unsigned int j = 0; j < csv->columnsCount; j++)
            printf("\t%s", csv->matrix[i][j]);
    }
    printf("\n");
}

void fileSummary( csv_t* csv ){
    for (unsigned int i = 0; i < csv->columnsCount; i++){
        printf("\n%s ", csv->headerNames[i]);
        printf("%s", csv->headerTypes[i]);
    }
    printf("\n%d variaveis encontradas", csv->columnsCount);
    printf("\n");
}

void freeMatrix( char*** matrix, unsigned int columnsCount, unsigned int lineCount ){
    for (unsigned int i = 0; i < lineCount; i++){
        for (unsigned int j = 0; j < columnsCount; j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
};

void freeHeader( char** types, char** names, unsigned int columnsCount ){
    for (unsigned int i = 0; i < columnsCount; i++){
        free(types[i]);
        free(names[i]);
    }
    free(types);
    free(names);
}

void freeCSV( csv_t* csv ){
    freeMatrix(csv->matrix, csv->columnsCount, csv->lineCount);
    freeHeader(csv->headerTypes, csv->headerNames, csv->columnsCount);
    fclose(csv->csv_file);
    free(csv);
}