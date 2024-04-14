#include "../include/io.h"

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

