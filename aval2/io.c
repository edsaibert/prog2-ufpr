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

    csv->headerNames = NULL;
    csv->headerTypes = NULL;
    csv->matrix = NULL;
    strcpy(csv->delimiter, DELIMITER);
    csv->columnsCount = 0;
    csv->lineCount = 0;

   return csv; 

};


// Função que lê os headers e também determina seu tipo
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
 
// Função que adiciona os indexes de cada linha
int addToIndex( csv_t* csv ){
    if (!(csv->index = (int*) malloc(csv->lineCount * sizeof(int)))) return 0; 

    for (unsigned int i = 0; i < csv->lineCount; i++){
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
                strcpy(column, "NaN");

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
void formatAsTable( char** f, int* maxStrlen, unsigned int columnsCount, char** string ){
    unsigned int i = 0;
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
int showFile( char*** matrix, int* index, int lineCount, int columnsCount ){
    char* f[columnsCount];
    int* maxStrlen;

    maxStrlen = (int *)malloc(columnsCount * sizeof(int));
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

        return 1;
    }

    // Caso não esteja
    for (unsigned int j = 0; j < columnsCount; j++) {
        maxStrlen[j] = 0;

        for (unsigned int i = 0; i < lineCount; i++) {
            int current = strlen(matrix[i][j]);
            if (current > maxStrlen[j]) {
                maxStrlen[j] = current;
            }
        }
    }

    for(int i = 0; i < columnsCount; i++) f[i] = (char*) malloc(maxStrlen[i] + 1);

    if (lineCount <= 10){
        for (int i = 0; i < lineCount; i++){
            formatAsTable(f, maxStrlen, columnsCount, matrix[i]);

            printf("%-*d", columnsCount, index[i]);

            for (int j = 0; j < columnsCount; j++)
                printf("\t%s", f[j]);
            printf("\n");
        }
    } 
    else {
        for (int i = 0; i < lineCount; i++){
            if (i <= 5 || i >= lineCount-5){
                formatAsTable(f, maxStrlen, columnsCount, matrix[i]);

                printf("%-*d", columnsCount, index[i]);

                for (int j = 0; j < columnsCount; j++)
                    printf("\t%s", f[j]);
                printf("\n");
            }
        }
    }
    for (int i = 0; i < columnsCount; i++)
        free(f[i]);
    free(maxStrlen);

    printf("[%d rows] x [%d columns]", lineCount-1, columnsCount);
    return 1;
}

// Popula um arquivo com os dados de uma matriz
void populateFile( char*** matrix, int lineCount, int columnsCount ){
    char path[STRING_BUFFER];

    printf("Entre com o nome do arquivo: ");
    getchar();
    fgets(path, STRING_BUFFER, stdin);
    printf("\n");

    FILE* file = createFile(path, "w+");
    for (unsigned int i = 0; i < lineCount; i++){
        for (unsigned int j = 0; j < columnsCount; j++){
            fprintf(file, "%s", matrix[i][j]);
            if (j != columnsCount - 1) fprintf(file, ",");
        }
    }

    printf("Arquivo gravado com sucesso.\n");
    fclose(file);
};

// Free somente nas linhas que não me interessam (que não estam em index)
void conditionalFree( csv_t* csv, char*** newMatrix, int* index, int newLineCount ){
    for (int i = 0; i < csv->lineCount; i++){
        if (index[i] == -1){
            for (int j = 0; j < csv->columnsCount; j++){
                free(csv->matrix[i][j]);
            }
            free(csv->matrix[i]);
        }
    }
    free(csv->matrix);

    csv->lineCount = newLineCount;
    csv->matrix = newMatrix;
}


// Função que filta o arquivo
int filterFile( csv_t* csv, int index, char* value, int (*func)(char* a, char* b)){
    char choice;
    char ***aux = (char***) malloc(csv->lineCount * csv->columnsCount * sizeof(char));
    int auxIndex[csv->lineCount];
    int newLineCount = 1;

    if (!aux){
        perror("Alocação de memória falhou.");
        return 0;
    }

    // Headers
    aux[0] = csv->matrix[0];
    auxIndex[0] = csv->index[0];
    for (unsigned int j = 0; j < csv->columnsCount; j++){
        aux[0][j] = csv->matrix[0][j];
    }

    for (unsigned int i = 1; i < csv->lineCount; i++){
        if (func(csv->matrix[i][index], value)) {

            aux[newLineCount] = csv->matrix[i];
            auxIndex[newLineCount] = csv->index[i];
            printf("%d\n", auxIndex[newLineCount]);
            for (unsigned int j = 0; j < csv->columnsCount; j++){
                aux[newLineCount][j] = csv->matrix[i][j];
            }
            newLineCount++;
        }
    }

    showFile(aux, auxIndex, newLineCount, csv->columnsCount);

    printf("\nDeseja gravar um arquivo com os dados filtrados? [S|N]: ");
    scanf(" %c", &choice);

    // Caso o usuario escolha salvar o arquivo
    if (choice == 'S'){
        populateFile(aux, newLineCount, csv->columnsCount);
    } 

    printf("Deseja descartar os dados originais? [S|N]: ");
    scanf(" %c", &choice);

    if (choice == 'S'){
        conditionalFree(csv, aux, auxIndex, newLineCount);
        return 1;
    }
    printf("\n");

    // Free da matriz auxiliar
    free(aux);

    return 1;
}

// Controla a entrada principal para o filtro
void filterEntry( csv_t* csv ){
    char ctrl[3 * sizeof(char)];
    char choice;
    char column[STRING_BUFFER];
    char value[STRING_BUFFER];

    unsigned int i = 0;

    printf("\nEntre com a variável: ");
    fgets(column, STRING_BUFFER, stdin);

    //Remove o /n
    column[strlen(column) - 1] = '\0';

    // Encontra a coluna a ser filtrada
    while (i < csv->columnsCount && eq(csv->headerNames[i], column) != 1) i++;

    // Se a variável não for encontrada
    if (i == csv->columnsCount && eq(csv->headerNames[i-1], column) != 1) {
        perror("Variável não encontrada.");
        return;
    }

    printf("Escolha um filtro ( == > >= < <= != ): ");
    fgets(ctrl, sizeof(ctrl), stdin);

    getchar();
    printf("Digite um valor: ");
    fgets(value, sizeof(value), stdin);
    printf("\n");

    // define o operador
    if      (eq(ctrl, "==")) filterFile( csv, i, value, eq);
    else if (eq(ctrl, "!=")) filterFile( csv, i, value, neq);
    else if (eq(ctrl, ">=")) filterFile( csv, i, value, egt);
    else if (eq(ctrl, "> ")) filterFile( csv, i, value, gt);
    else if (eq(ctrl, "<=")) filterFile( csv, i, value, elt);
    else if (eq(ctrl, "< ")) filterFile( csv, i, value, lt);

    return;
}


// Retorna o summary das colunas do arquivo
void fileSummary( csv_t* csv ){
    for (unsigned int i = 0; i < csv->columnsCount; i++){
        printf("\n%s ", csv->headerNames[i]);
        printf("%s", csv->headerTypes[i]);
    }
    printf("\n%d variaveis encontradas", csv->columnsCount);
    printf("\n");
}

// Faz o free de uma matriz qualquer
void freeMatrix( char*** matrix, unsigned int columnsCount, unsigned int lineCount ){
    for (unsigned int i = 0; i < lineCount; i++){
        for (unsigned int j = 0; j < columnsCount; j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
};

// Faz o free dos headers do csv_t
void freeHeader( char** types, char** names, unsigned int columnsCount ){
    for (unsigned int i = 0; i < columnsCount; i++){
        free(types[i]);
        free(names[i]);
    }
    free(types);
    free(names);
}

// Faz o free do CSV
void freeCSV( csv_t* csv ){
    freeMatrix(csv->matrix, csv->columnsCount, csv->lineCount);
    freeHeader(csv->headerTypes, csv->headerNames, csv->columnsCount);
    free(csv->index);
    fclose(csv->csv_file);
    free(csv);
}
