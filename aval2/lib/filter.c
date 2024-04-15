#include "../include/filter.h"

// Free somente nas linhas que não me interessam (que não estam em index)
void conditionalFree_f( csv_t* csv, char*** newMatrix, long int* index, unsigned long int newLineCount ){
    for (unsigned long int i = 0; i < csv->lineCount; i++){
        if (index[i] == -1){

            for (unsigned int j = 0; j < csv->columnsCount; j++){
                free(csv->matrix[i][j]);
                csv->matrix[i][j] = NULL;
        }
            free(csv->matrix[i]);
            csv->matrix[i] = NULL;
        }
    }

    // Deslocar ponteiros não nulos para o começo
    unsigned long int j = 0;
    for (unsigned long int i = 0; i < csv->lineCount; i++){
        if (csv->matrix[i] != NULL){
            if (i != j){
                csv->index[j] = csv->index[i];
                csv->matrix[j] = csv->matrix[i];
                csv->matrix[i] = NULL;
            }
            j++;
        }
    }

    // free dos ultimos caracteres
    free(csv->matrix);
    csv->lineCount = newLineCount;
    csv->matrix = newMatrix;
}

// Função que filta o arquivo
int filterFile( csv_t* csv, long int index, char* value, int (*func)(const void*, const void*)){
    char *choice;
    char ***aux = (char***) malloc(csv->lineCount * sizeof(char**));

    long int *auxIndex = (long int*) malloc(csv->lineCount * sizeof(long int));
    long int *mask = (long int*) malloc(csv->lineCount * sizeof(long int));

    int newLineCount = 1;

    if (!aux || !auxIndex || !mask){
        perror("Alocação de memória falhou.");
        return 0;
    }

    // Headers
    aux[0] = csv->matrix[0];
    auxIndex[0] = csv->index[0];
    for (unsigned int j = 0; j < csv->columnsCount; j++){
        aux[0][j] = csv->matrix[0][j];
    }

    for (unsigned long int i = 1; i < csv->lineCount; i++){
        if (func(csv->matrix[i][index], value)) {

            aux[newLineCount] = csv->matrix[i];
            auxIndex[newLineCount] = csv->index[i];
            mask[i] = i;
            for (unsigned int j = 0; j < csv->columnsCount; j++){
                aux[newLineCount][j] = csv->matrix[i][j];
            }
            newLineCount++;
        }
        else mask[i] = -1;
    }

    showFile(aux, auxIndex, newLineCount, csv->columnsCount);

    choice = userInput("Deseja gravar um arquivo com os dados filtrados? [S|N]: ", 4 * sizeof(char));
    // Caso o usuario escolha salvar o arquivo
    if (choice[0] == 'S' || choice[0] == 's'){
        populateFile(aux, newLineCount, csv->columnsCount);
    } 
    free(choice);

    choice = userInput("Deseja descartar os dados originais? [S|N]: ", 4 * sizeof(char));
    if (choice[0] == 'S' || choice[0] == 's'){
        conditionalFree_f(csv, aux, mask, newLineCount);
    }
    printf("\n");

    free(auxIndex);
    free(mask);
    free(choice);

    return 1;
}

// Controla a entrada principal para o filtro
void filterEntry( csv_t* csv ){
    char *ctrl;
    char *column;
    char *value;
    unsigned int i;

    column = userInput("Entre com a variável: ", STRING_BUFFER);

    i = columnSearch(csv, column);
    // Se a variável não for encontrada
    if (i == -1){
        free(column);
        return;
    }

    ctrl = userInput("Escolha um filtro ( == > >= < <= != ): ", 4 * sizeof(char));
    value = userInput("Digite um valor: ", STRING_BUFFER);

    if      (strcmp(ctrl, "==") == 0) filterFile(csv, i, value, seq);
    else if (strcmp(ctrl, "!=") == 0) filterFile(csv, i, value, sneq);
    else if (strcmp(ctrl, ">=") == 0) filterFile(csv, i, value, segt);
    else if (strcmp(ctrl, "> ") == 0) filterFile(csv, i, value, sgt);
    else if (strcmp(ctrl, "<=") == 0) filterFile(csv, i, value, selt);
    else if (strcmp(ctrl, "< ") == 0) filterFile(csv, i, value, slt);

    free(column);
    free(ctrl);
    free(value);
    return;
}
