#include "io.h"

char*** countCSV(FILE* csv, int* columnsCount, int* lineCount){
    char line[CSV_BUFFER];
    int times = 0;
    *lineCount = 0;

    fseek(csv, 0, SEEK_END);
    long int res = ftell(csv);

    // Cria uma matriz de strings para armazenar o conteúdo do arquivo
    char ***matrix;
    matrix = (char***) malloc(res * sizeof(char)); 

    fseek(csv, 0, SEEK_SET);

    while(fgets(line, sizeof(line), csv)){

        // strtok não conta caracteres nulos
        char *column = strtok(line, DELIMITER);

        *columnsCount = 0;
        times = 0;

        matrix[*lineCount] = (char**) malloc(res * sizeof(char));

        while (column){

            matrix[*lineCount][*columnsCount] = (char *)malloc(strlen(column) * sizeof(char));
            strcpy(matrix[*lineCount][*columnsCount], column);

            column = strtok(NULL, DELIMITER);
            (*columnsCount)++;
        }
        (*lineCount)++;
    }

    return matrix;
}


int fileSummary(char*** matrix, int columnsCount, int lineCount) {
    // Retorna os valores do título das colunas (primeira linha)
    for (int i = 0; i < columnsCount; i++){
        printf("%s ", matrix[0][i]);

    // Má implementação, necessário ter pelo menos uma linha para funcionar
        if (isdigit(matrix[1][i][1])){
            printf("[N]\n", i);
        }
        else
            printf("[S]\n", i);
    }

    printf("\n");
    printf("%d variáveis encontradas", columnsCount);
    printf("\b\n\n");

    return 0;
}

int showFile( char*** matrix, int columnsCount, int lineCount){
    int j;
    int i = 0;

    while ((i < lineCount) && (i < 5)) {
        j = 0;
        printf("%d\t", i);
        while ((j < columnsCount)) {
            printf("%-30s", matrix[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }

    printf("...");
    printf("\n");

    int lastFive = lineCount - 5;
    if (lastFive < i) lastFive = i;

    i = lastFive;

    while ((i < lineCount) && (i < (lineCount + 5))) {
        j = 0;
        printf("%d\t", i);
        while (j < columnsCount) {
            printf("%-30s", matrix[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
    printf("\n");
    printf("[ %d rows x %d columns ]\n\n", lineCount, columnsCount);
}

int readFile( char path[STRING_BUFFER], int choice ){
    FILE *csv;
    int columnsCount = 0;
    int lineCount = 0;
    char ***matrix;

    csv = fopen(path, "r+");

    if (csv == NULL) {
        perror("Arquivo incorreto ou não existente.");
        return 0;
    }

    matrix = countCSV(csv, &columnsCount, &lineCount);
    if (choice == 1) fileSummary(matrix, columnsCount, lineCount);
    if (choice == 2) showFile(matrix, columnsCount, lineCount);

    return 1;
};

