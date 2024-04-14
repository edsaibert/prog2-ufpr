#include "io.h"

// Cria um arquivo, ou abre um arquivo existente
FILE* createFile( char* path, char* mode ){
    FILE* file = fopen(path, mode);
    if (!file){
        perror("Erro ao abrir o arquivo.");
        return NULL;
    }
    return file;
}

// Inicializa a estrutura csv_t
csv_t* inicializeCSV( char* path ){

    csv_t* csv = (csv_t*) malloc(sizeof(csv_t));
    if (!csv){
        perror("Alocação de memória falhou.");
        return NULL;
    }


    csv->csv_file = createFile(path, "r");

    csv->matrix = NULL;
    strcpy(csv->delimiter, DELIMITER);
    csv->columnsCount = 0;
    csv->lineCount = 0;

   return csv; 

};


// Função que lê os headers e também determina seu tipo
int addToHeader( csv_t* csv ){
    // Alocação do vetor
    csv->headerTypes = (int*) malloc(csv->columnsCount * sizeof(int));

    if (!csv->headerTypes){
        perror("Alocação de memória falhou.");
        return 0;
    }

    for (unsigned int i = 0; i < csv->columnsCount; i++){

        // Gambiarra se o primeiro valor é NaN
        int j = 1;
        while (strcmp((csv->matrix[j][i]), "") == 0){
            j++;
        }

        // Copia os tipos das colunas do header para o vetor headerNames
        if (isdigit(csv->matrix[j][i][1]))
            csv->headerTypes[i] = 0; // Vetor de char 
        else 
            csv->headerTypes[i] = 1; // Vetor de float 
    }
    return 1;
}
 
// Função que adiciona os indexes de cada linha
int addToIndex( csv_t* csv ){
    if (!(csv->index = (long int*) malloc(csv->lineCount * sizeof(long int)))) return 0; 

    for (unsigned long int i = 0; i < csv->lineCount; i++){
        csv->index[i] = i;
    }

    return 1;
}

// Retorna o tamanho do arquivo
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

// Função que aloca o arquivo CSV em memória de programa, por meio de uma matriz
int readCSV( csv_t *csv ){
    char line[CSV_BUFFER];
    unsigned long int size = fileSize(csv->csv_file);
    csv->fileSize = size;

    // Cria uma matriz de strings para armazenar o conteúdo do arquivo
    if (!(csv->matrix = (char***) malloc(size * sizeof(char**)))) return 0; 

    while (fgets(line, sizeof(line), csv->csv_file)){
        char *column, *p = line;
        // Aloca espaço para a linha da matriz
        if (!(csv->matrix[csv->lineCount] = (char **)malloc(STRING_BUFFER * sizeof(char*))))
            return 0;

        csv->columnsCount = 0;

        while ((column = strsep(&p, csv->delimiter))){

            if (!column) // Se a coluna for nula, atribui NaN
                strcpy(column, "NaN");// Remove o '\n' do final da string

            // Aloca espaço para o item da matriz
            if (!(csv->matrix[csv->lineCount][csv->columnsCount] = (char *)malloc((strlen(column) + 1) * sizeof(char))))
                return 0;

            // Copia o conteúdo da coluna para o espaço [i, k] da matriz
            strcpy(csv->matrix[csv->lineCount][csv->columnsCount], column);

            (csv->columnsCount)++; // Incrementa o número de colunas
        }
        (csv->lineCount)++; // Incrementa o número de linhas
    }

    if(!addToIndex(csv)) return 0;
    if (!addToHeader(csv)) return 0;
    return 1;

}

// Função que formata cada item da tabela conforme o maior item de sua coluna
void formatAsTable( char** f, long unsigned int* maxStrlen, unsigned int columnsCount, char** string ){
    unsigned long int i = 0;
    unsigned int j;

    while(1){
        for (j = 0; j < maxStrlen[i] - strlen(string[i]); j++) f[i][j] = ' ';
        strcpy(f[i] + j, string[i]);

        i++;
        if ((i == columnsCount) || (!string[i])) break;
    }
    for(; i < columnsCount; i++) f[i][0] = '\0';
}

// Função que descreve os itens do arquivo CSV por meio de um DataFrame
int showFile( char*** matrix, long int* index, unsigned long int lineCount, unsigned int columnsCount ){
    char* f[columnsCount];
    unsigned long int* maxStrlen;

    maxStrlen = (unsigned long int *)malloc(columnsCount * sizeof(long int));
    if (maxStrlen == NULL) {
        perror("Alocação de memória falhou.");
        return 0;
    }

    // Se a tabela for vazia
    if (lineCount == 1){
        printf("Empty DataFrame\n");
        printf("Columns: [");
        for (int i = 0; i < columnsCount-1; i++){printf("%s, ", matrix[0][i]);}
        printf("%s]\n", matrix[0][columnsCount-1]);
        printf("Index: []");

        free(maxStrlen);
        return 1;
    }

    // Caso não esteja
    for (unsigned int j = 0; j < columnsCount; j++) {
        maxStrlen[j] = 0;

        for (unsigned long int i = 0; i < lineCount; i++) {
            int current = strlen(matrix[i][j]);
            if (current > maxStrlen[j]) {
                maxStrlen[j] = current;
            }
        }
    }

    for(int i = 0; i < columnsCount; i++) f[i] = (char*) malloc(maxStrlen[i] + 1);

    if (lineCount <= 10){
        for (unsigned long int i = 0; i < lineCount; i++){
            formatAsTable(f, maxStrlen, columnsCount, matrix[i]);

            // Se for a primeira linha, imprime os headers
            if (i == 0) printf("%-*s", columnsCount, "");
            else printf("%-*ld", columnsCount, index[i-1]);

            for (unsigned int j = 0; j < columnsCount; j++)
                printf("\t%s", f[j]);
            printf("\n");
        }
    } 
    else {
        for (unsigned long int i = 0; i < lineCount; i++){
            if (i <= 5 || i >= lineCount-5){
                formatAsTable(f, maxStrlen, columnsCount, matrix[i]);

                // Se for a primeira linha, imprime os headers
                if (i == 0) printf("%-*s", columnsCount, "");
                else printf("%-*ld", columnsCount, index[i - 1]);

                for (int j = 0; j < columnsCount; j++)
                    printf("\t%s", f[j]);
                printf("\n");
            }
        }
    }
    for (unsigned long int i = 0; i < columnsCount; i++)
        free(f[i]);
    free(maxStrlen);

    printf("[%ld rows] x [%d columns]\n", lineCount-1, columnsCount);
    return 1;
}

// Popula um arquivo com os dados de uma matriz
void populateFile( char*** matrix, unsigned long int lineCount, unsigned int columnsCount ){
    char path[STRING_BUFFER];

    printf("Entre com o nome do arquivo: ");
    getchar();
    fgets(path, STRING_BUFFER, stdin);
    printf("\n");

    FILE* file = createFile(path, "w+");
    for (unsigned long int i = 0; i < lineCount; i++){
        for (unsigned int j = 0; j < columnsCount; j++){
            fprintf(file, "%s", matrix[i][j]);
            if (j != columnsCount - 1) fprintf(file, ",");
        }
    }

    printf("Arquivo gravado com sucesso.\n");
    fclose(file);
};

// Retorna o summary das colunas do arquivo
void fileSummary( csv_t* csv ){
    for (unsigned int i = 0; i < csv->columnsCount; i++){

        printf("%s ", csv->matrix[0][i]);
        if (csv->headerTypes[i] == 1)
            printf("[S]\n");
        else 
            printf("[N]\n");
    }
    printf("\n%d variaveis encontradas", csv->columnsCount);
    printf("\n");
}

// Free somente nas linhas que não me interessam (que não estam em index)
void conditionalFree( csv_t* csv, char*** newMatrix, long int* index, unsigned long int newLineCount ){
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

/* Print de uma mensagem seguida de um input do usuario */
char* userInput( char* message, int size ){
    char *aux = (char*) malloc(size * sizeof(char));
    memset(aux, 0, size);

    printf("%s", message);
    fgets(aux, size, stdin);
    //Remove o /n
    aux[strlen(aux) - 1] = '\0';

    printf("\n");
    return aux;
}

// Função que busca a coluna a ser filtrada. Se encontrada, retorna o índice
int columnSearch( csv_t* csv, char* column ){
    unsigned int i = 0;

    // Encontra a coluna a ser filtrada
    while (i < csv->columnsCount && eq(csv->matrix[0][i], column) != 1) i++;

    // Se a variável não for encontrada
    if (i == csv->columnsCount && eq(csv->matrix[0][i-1], column) != 1) {
        perror("Variável não encontrada.");
        return -1;
    }
    return i;
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
        conditionalFree(csv, aux, mask, newLineCount);
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

    if (eq(ctrl, "=="))      filterFile(csv, i, value, eq);
    else if (eq(ctrl, "!=")) filterFile(csv, i, value, neq);
    else if (eq(ctrl, ">=")) filterFile(csv, i, value, egt);
    else if (eq(ctrl, "> ")) filterFile(csv, i, value, gt);
    else if (eq(ctrl, "<=")) filterFile(csv, i, value, elt);
    else if (eq(ctrl, "< ")) filterFile(csv, i, value, lt);

    free(column);
    free(ctrl);
    free(value);
    return;
}


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

// Função que retorna a moda de uma coluna e os valores únicos
char* trend( csv_t* csv, unsigned int j, char** unique ){
    char *mostCommon = NULL;
    int max = 0;

    hash_t *hash = (hash_t *)malloc(sizeof(hash_t) * csv->lineCount);
    if (!hash){
        perror("Alocação de memória falhou.");
        return NULL;
   }

   // adiciona os valores da coluna j ao hash
   for (unsigned long int i = 1; i < csv->lineCount; i++){
        hash[i].item = csv->matrix[i][j];
        hash[i].count = 0;
   }

   for (unsigned long int i = 1; i < csv->lineCount; i++){
        for (unsigned long int k = 1; k < csv->lineCount; k++){
            if (eq(hash[i].item, csv->matrix[k][j])){
                hash[i].count++;
                break;
            }
        }
   }

   // Encontra o valor mais comum / valores unicos   
    for (unsigned int i = 0; i < csv->lineCount; i++){
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
char* mean( csv_t* csv, unsigned int j ){
    char* mean = (char*) malloc(STRING_BUFFER);
    char* value;
    float sum = 0;

    if (!mean){
        perror("Alocação de memória falhou.");
        return NULL;
    }

    for (unsigned long int i = 1; i < csv->lineCount; i++){
        value = csv->matrix[i][j];
        if (value != NULL)
            sum += atof(csv->matrix[i][j]);
    }

    sum /= csv->lineCount;
    sprintf(mean, "%f", sum);
    return mean;
}

/*  desvio padrao   */
char* stDeviantion( csv_t* csv, char* charMean, unsigned int j){
    char* stDev = (char*) malloc(STRING_BUFFER * sizeof(char));
    float sum = 0;
    float mean = atof(charMean);
    int count = 0;

    for (unsigned long int i = 1; i < csv->lineCount; i++){
        sum += pow(atof(csv->matrix[i][j]) - mean, 2);
        count++;
    }

    if (count == 0) return "";
    sum /= count;
    sum = sqrt(sum);
    sprintf(stDev, "%f", sum);
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
        sprintf(median, "%f", result);
    } else {
        sprintf(median, "%s", matrix[(lineCount/2) + 1][j]);
    }

    return median;
}

void stat( csv_t* csv ){
    char **unique = (char**) malloc(csv->lineCount * sizeof(char*));
    char ***auxMatrix = (char***) malloc(csv->lineCount * sizeof(char**));

    char *choice, *meanResult, *stdev, *medianResult;
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
    printf("Moda: %s\n", trend(csv, i, unique));

    if (isNumeric){
        // Media
        meanResult = mean(csv, i);
        printf("Media: %s\n", meanResult);

        stdev = stDeviantion(csv, meanResult, i);

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
