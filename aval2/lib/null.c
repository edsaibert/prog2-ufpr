#include "../include/null.h"


int filterNan( csv_t* csv, int optionShow, int optionRemove ){
    char ***aux = (char***) malloc(csv->lineCount * sizeof(char**));
    long int *auxIndex = (long int *)malloc(csv->lineCount * sizeof(long int));
    long int *mask = (long int *)malloc(csv->lineCount * sizeof(long int));
    int newLineCount, foundNaN;

    if (!aux || !auxIndex ) {
        perror("Alocação de memória falhou.");
        return 0;
    }

    // Headers
    auxIndex[0] = csv->index[0];
    aux[0] = csv->matrix[0];

    newLineCount = 1;
    for (unsigned long int i = 1; i < csv->lineCount; i++){
        foundNaN = 0; 
        for (int j = 0; j < csv->columnsCount; j++){
            if (eq(csv->matrix[i][j], "NaN")){
                foundNaN = 1;
                break;
            }
        }
        if (foundNaN){
            auxIndex[newLineCount] = csv->index[i];
            mask[i] = -1;
            aux[newLineCount] = csv->matrix[i];
            newLineCount++;
        }
        else {
            mask[i] = i;
        }
    }

    if (optionShow)
        showFile(aux, auxIndex, newLineCount, csv->columnsCount);

    if (optionRemove)
        conditionalFree_f(csv, aux, mask, newLineCount);

    free(auxIndex);
    free(mask);
    return 1;
}

// Subistitui valores NaN pela media
void meanSubstitute( char*** matrix, unsigned long int lineCount, unsigned int columnsCount ){
    char* meanResult = NULL;
    int eqresult = 0;

    for (int j = 0; j < columnsCount; j++){
        meanResult = mean(matrix, lineCount, j);
        for (int i = 0; i < lineCount; i++){
            eqresult = eq(matrix[i][j], "NaN");
            if (eqresult){
                matrix[i][j] = realloc(matrix[i][j], strlen(meanResult) + 1);
                if (matrix[i][j] != NULL) {
                    strcpy(matrix[i][j], meanResult);
                }
            }
        }
        free(meanResult);
    }
}

// Substitui valores NaN pelo proximo valor numerico
void proxSubstitute( char*** matrix, unsigned long int lineCount, unsigned int columnsCount ){

    for (int i = 0; i < lineCount; i++){
        for (int j = 0; j < columnsCount; j++){
            if (eq(matrix[i][j], "NaN")){
                unsigned int k = i+1;
                if (i == lineCount-1){
                    while (k != 1 && eq(matrix[k][j], "NaN"))
                        k++;
                } else {
                    while (k != lineCount - 1 && eq(matrix[k][j], "NaN"))
                        k++;
                }
                matrix[i][j] = realloc(matrix[i][j], strlen(matrix[k][j]) + 1);
                strcpy(matrix[i][j], matrix[k][j]);
            }
        }
    }

}


void switchOption( csv_t* csv ){
    char ctrl;
    printf("\n1)\t Listar registros com NaN\n");
    printf("2)\t Substituir pela média\n");
    printf("3)\t Substituir pelo mais próximo\n");
    printf("4)\t Remover\n");
    printf("5)\t Sair\n");
    printf("Digite uma opção: ");
    ctrl = getchar();
    getchar();
    printf("\n");

    switch(ctrl){
        case '1':
            filterNan(csv, 1, 0);
            break;
        case '2':
            meanSubstitute(csv->matrix, csv->lineCount, csv->columnsCount);
            break;
        case '3':
            proxSubstitute(csv->matrix, csv->lineCount, csv->columnsCount);
            break;
        case '4':
            filterNan(csv, 0, 1);
            break;
        case '5':
            break;
    }
}