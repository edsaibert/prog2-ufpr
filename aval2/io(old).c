#include "io.h"

char*** matrixIsEmpty(char ***matriz){
    if (matriz == NULL){
        perror("Alocação de memória falhou.");
        return NULL;
    }
    return matriz;
}

char*** readCSV(FILE* csv, int* columnsCount, int* lineCount){
    char line[CSV_BUFFER];
    *lineCount = 0;

    fseek(csv, 0, SEEK_END);
    long int res = ftell(csv);

    // Cria uma matriz de strings para armazenar o conteúdo do arquivo
    char ***matrix;
    matrix = (char***) malloc(res * sizeof(char)); 
    matrixIsEmpty(matrix);

    fseek(csv, 0, SEEK_SET);


    while (fgets(line, sizeof(line), csv))
    {
        char *column, *p = line;
        *columnsCount = 0;

        matrix[*lineCount] = (char **)malloc(STRING_BUFFER * sizeof(char));

        while ((column = strsep(&p, DELIMITER)))
        {

            matrix[*lineCount][*columnsCount] = (char *)malloc((strlen(column) + 1) * sizeof(char));
            strcpy(matrix[*lineCount][*columnsCount], column);

            // Se a coluna for nula, atribui NaN
            if (!column)
            {
                column = "NaN";
            }

            (*columnsCount)++;
        }
        (*lineCount)++;
    }

    return matrix;
}

void fileSummary(char*** matrix, int columnsCount, int lineCount) {
    // Retorna os valores do título das colunas (primeira linha)
    for (int i = 0; i < columnsCount; i++){
        printf("\n%-20s \t", matrix[0][i]);

    // Má implementação, necessário ter pelo menos uma linha para funcionar
        if (isdigit(matrix[1][i][1])){
            printf("[N]");
        }
        else
            printf("[S]");
    }

    printf("\n");
    printf("%d variáveis encontradas", columnsCount);
    printf("\b\n\n");
}

void showFile( char*** matrix, int columnsCount, int lineCount){
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

    matrix = readCSV(csv, &columnsCount, &lineCount);

    if (matrixIsEmpty(matrix) == NULL) return 0;

    if (choice == 1) fileSummary(matrix, columnsCount, lineCount);
    if (choice == 2) showFile(matrix, columnsCount, lineCount);

    return 1;
};

void freeMatrix(char*** matrix, int lineCount, int columnsCount){
    for (int i = 0; i < lineCount; i++){
        for (int j = 0; j < columnsCount; j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
}
