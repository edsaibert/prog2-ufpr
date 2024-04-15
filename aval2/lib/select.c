#include "../include/select.h"

void selectFile( csv_t* csv ){
    char ***aux = (char***) malloc(csv->lineCount * sizeof(char**));
    char* column;
    char* choice;
    unsigned int j;
    unsigned int count = 0;

    // Lê as colunas a serem selecionadas
    char *line;
    line = userInput("Entre com a variaveis que deseja selecionar (separadas por espaço): ", CSV_BUFFER);

    for (unsigned long int i = 0; i < csv->lineCount; i++){
        aux[i] = (char**)malloc(csv->columnsCount * sizeof(char*));
        if (!aux[i]) {
            perror("Alocação de memória falhou.");
            free(line);
            return;
        }
    }
     // Separa as colunas
    
    char* token = strtok(line, " ,\t\n");
    while (token != NULL) {
        column = token;
        j = columnSearch(csv, column);
        if (j == -1) {
            perror("Coluna não encontrada.");
            free(line);
            return;
        }
        count++;

        for (unsigned long int i = 0; i < csv->lineCount; i++) {
            aux[i][count-1] = csv->matrix[i][j];
        }
        token = strtok(NULL, " ,\t\n");
    }

    // Print 
    showFile(aux, csv->index, csv->lineCount, count);

    choice = userInput("Deseja gravar um arquivo com as variáveis selecionadas? [S|N] ", 3);
    if (choice[0] == 'S' || choice[0] == 's')
        populateFile(aux, csv->lineCount, count);

    // Libera memória
    for (unsigned long int i = 0; i < csv->lineCount; i++) {
        free(aux[i]);
    }
    free(aux);
    free(line);
    free(choice);
}