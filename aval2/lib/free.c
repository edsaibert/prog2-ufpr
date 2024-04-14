#include "../include/free.h"

// Faz o free de uma matriz qualquer
void freeMatrix( char*** matrix, unsigned int columnsCount, unsigned long int lineCount ){
    for (unsigned long int i = 0; i < lineCount; i++){
        for (unsigned int j = 0; j < columnsCount; j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
};

// Faz o free do CSV
void freeCSV( csv_t* csv ){
    freeMatrix(csv->matrix, csv->columnsCount, csv->lineCount);
    // freeHeader(csv->headerTypes, csv->columnsCount);
    free(csv->headerTypes);
    free(csv->index);
    fclose(csv->csv_file);
    free(csv);
}

