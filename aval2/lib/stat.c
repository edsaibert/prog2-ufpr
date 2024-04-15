#include "../include/stat.h"

// Função que retorna a moda de uma coluna e os valores únicos
char* trend( char ***matrix, unsigned long int lineCount, unsigned int j, char** unique ){
    char *mostCommon = NULL;
    int max = 0;

    hash_t *hash = (hash_t *)malloc(sizeof(hash_t) * lineCount);
    if (!hash){
        perror("Alocação de memória falhou.");
        return NULL;
   }

   // adiciona os valores da coluna j ao hash
   for (unsigned long int i = 1; i < lineCount; i++){
        hash[i].item = matrix[i][j];
        hash[i].count = 0;
   }

   for (unsigned long int i = 1; i < lineCount; i++){
        for (unsigned long int k = 1; k < lineCount; k++){
            if (eq(hash[i].item, matrix[k][j])){
                hash[i].count++;
                break;
            }
        }
   }

   // Encontra o valor mais comum / valores unicos   
    for (unsigned int i = 0; i < lineCount; i++){
        if (hash[i].count > max){
            max = hash[i].count;
            mostCommon = hash[i].item;
        } else if (hash[i].count == 1){
            unique[i] = hash[i].item;
        }
    }

    free(hash);
    return mostCommon;
}

/*  media de uma coluna */
char* mean( char*** matrix, unsigned long int lineCount, unsigned int j ){
    char* meanResult = (char*) malloc(STRING_BUFFER);
    char* value = NULL;
    float sum = 0;

    if (!meanResult){
        perror("Alocação de memória falhou.");
        return NULL;
    }

    for (unsigned long int i = 1; i < lineCount; i++){
        value = matrix[i][j];
        if (neq(value, "NaN"))
            sum += atof(value);
    }

    sum /= lineCount;
    sprintf(meanResult, "%.2f", sum);
    return meanResult;
}

/*  desvio padrao   */
char* stDeviantion( char*** matrix, unsigned long int lineCount, char* charMean, unsigned int j){
    char* stDev = (char*) malloc(STRING_BUFFER * sizeof(char));
    float sum = 0;
    float mean = atof(charMean);
    int count = 0;

    for (unsigned long int i = 1; i < lineCount; i++){
        sum += pow(atof(matrix[i][j]) - mean, 2);
        count++;
    }

    if (count == 0) return "";
    sum /= count;
    sum = sqrt(sum);
    sprintf(stDev, "%.2f", sum);
    return stDev;
}

char* median( char*** matrix, unsigned long int lineCount, unsigned int j ){
    char* median = (char*) malloc(STRING_BUFFER);

    if (!median){
        perror("Alocação de memória falhou.");
        return 0;
    }

    if (lineCount % 2 == 0){
        float a = atof(matrix[lineCount/2][j]);
        float b = atof(matrix[(lineCount/2) + 1][j]);
        float result = (a + b) / 2;
        sprintf(median, "%.2f", result);
    } else {
        sprintf(median, "%s", matrix[(lineCount/2) + 1][j]);
    }

    return median;
}

void stat( csv_t* csv ){
    char **unique = (char**) malloc(csv->lineCount * sizeof(char*));
    char ***auxMatrix = (char***) malloc(csv->lineCount * sizeof(char**));

    char *choice = NULL;
    char *meanResult = NULL;
    char *stdev = NULL;
    char *medianResult = NULL;

    int isNumeric;

    if (!unique || !auxMatrix){ 
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

    // Verifica se a variável é numérica
    if (csv->headerTypes[i] != 0) isNumeric = 0;
    else isNumeric = 1;

    // Total de dados
    printf("Contador: %ld\n", csv->lineCount);

    // Moda
    printf("Moda: %s\n", trend(csv->matrix, csv->lineCount, i, unique));

    if (isNumeric){
        // Media
        meanResult = mean(csv->matrix, csv->lineCount, i);
        printf("Media: %s\n", meanResult);

        stdev = stDeviantion(csv->matrix, csv->lineCount, meanResult, i);

        for (unsigned long int i = 0; i < csv->lineCount; i++){
            auxMatrix[i] = csv->matrix[i];
        }
        for (unsigned long int l = 0; l < csv->lineCount; l++){
            quicksort(auxMatrix, 1, i, csv->lineCount, qs_cmp);
        }
        
        // Desvio padrao
        printf("Desvio padrao: %s\n", stdev);
        free(meanResult);
        free(stdev);

        // Mediana
        medianResult = median(auxMatrix, csv->lineCount-1, i);
        printf("Mediana: %s\n", medianResult);
        free(medianResult);

        // Minimo
        printf("Minimo: %s\n", auxMatrix[1][i]);

        // Maximo
        printf("Maximo: %s\n", auxMatrix[csv->lineCount-1][i]);

    }
    
    // N de valores unicos
    printf("Valores unicos: [");
    for (unsigned int i = 0; i < csv->lineCount-1; i++){
        if (unique[i] != NULL) printf("%s, ", unique[i]);
    }
    printf("%s]\n", unique[csv->lineCount-1]);

    free(unique);
    free(auxMatrix);
}