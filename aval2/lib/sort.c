#include "../include/sort.h"

/* Desloca os pointeiros das linhas de acordo com os indexes devolvidos pelo quicksort */
void shiftPointers( char*** auxMatrix, long int m, long int i){
    char** aux = auxMatrix[m];

    auxMatrix[m] = auxMatrix[i];
    auxMatrix[i] = aux;
}

int particioning ( char ***auxMatrix, long int i, unsigned int j, long int n, int (*cmp)(const void*, const void*)){
    if (n <= 0) return -1;

    char* pivot;
    pivot = auxMatrix[n-1][j];

    long int m = i-1;
    for (long int k = i; k < n; k++) {
        if (cmp(auxMatrix[k][j], pivot) < 0){
            m++;
            shiftPointers(auxMatrix, m, k);
        }
    }
    shiftPointers(auxMatrix, m+1, n-1);
    return m+1;
}

void quicksort( char ***auxMatrix, long int i, unsigned int j, long int n, int (*cmp)(const void*, const void*)){
    if (i >= n) return;

    long int m = particioning(auxMatrix, i, j, n, cmp);
    quicksort(auxMatrix, i, j, m, cmp);
    quicksort(auxMatrix, m+1, j, n, cmp);
    return;
}

// Ordenacao de uma matriz (utiliza o qsort_t [libc])
void sortFile( csv_t* csv ){
    char ***auxMatrix = (char***) malloc(csv->lineCount * sizeof(char**));
    char* choice;
    int (*compare)(const void*, const void*) ;

    if (!auxMatrix){
        perror("Alocação de memória falhou.");
        return;
    }
    choice = userInput("Entre com a variavel: ", STRING_BUFFER);

    unsigned int i = columnSearch(csv, choice);
    if (i == -1){
        perror("Variável não encontrada.");
        free(choice);
        return;
    }
    free(choice);

    choice = userInput("Selecione uma opcao [A]scendente ou [D]escrescente: ", 4 * sizeof(char));
    for (unsigned long int i = 0; i < csv->lineCount; i++) {
        auxMatrix[i] = csv->matrix[i];
    };

    if (choice[0] == 'A' || choice[0] == 'a')
        compare = qs_cmp;

    else if (choice[0] == 'D' || choice[0] == 'd')
        compare = qs_cmp_i;
    free(choice);

    for (unsigned long int l = 0; l < csv->lineCount; l++){
        quicksort(auxMatrix, 1, i, (csv->lineCount), compare);
    }

    showFile(auxMatrix, csv->index, csv->lineCount, csv->columnsCount);

    choice = userInput("Deseja gravar um arquivo com os dados filtrados? [S|N]: ", 4 * sizeof(char));
    // Caso o usuario escolha salvar o arquivo
    if (choice[0] == 'S' || choice[0] == 's'){
        populateFile(auxMatrix, csv->lineCount, csv->columnsCount);
    }
    free(choice);
    free(auxMatrix);

}