#include "io.h"

int eq(char* a, char* b){
    return strcmp(a, b) == 0;
}

int neq(char* a, char* b){
    return strcmp(a, b) != 0;
}

int egt(char* a, char* b){
    return strcmp(a, b) >= 0;
}

int gt(char* a, char* b){
    return strcmp(a, b) > 0;
}

int elt(char* a, char* b){
    return strcmp(a, b) <= 0;
}

int lt(char* a, char* b){
    return strcmp(a, b) < 0;
}

csv_t* inicializeCSV( char* path ){

    csv_t* csv = (csv_t*) malloc(sizeof(csv_t));
    if (!csv){
        perror("Alocação de memória falhou.");
        return NULL;
    }

    csv->csv_file = fopen(path, "r");
    if (!csv->csv_file){
        perror("Erro ao abrir o arquivo.");
        return NULL;
    }

    csv->headerNames = NULL;
    csv->headerTypes = NULL;
    csv->matrix = NULL;
    strcpy(csv->delimiter, DELIMITER);
    csv->columnsCount = 0;
    csv->lineCount = 0;

   return csv; 

};


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

int addToIndex( csv_t* csv ){
    if (!(csv->index = (int*) malloc(csv->lineCount * sizeof(int)))) return 0; 

    for (unsigned int i = 0; i < csv->lineCount; i++){
        csv->index[i] = i;
    }

    return 1;
}

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

int showFile( char*** matrix, int* index, int lineCount, int columnsCount ){
    char* f[columnsCount];
    int* maxStrlen;

    maxStrlen = (int *)malloc(columnsCount * sizeof(int));
    if (maxStrlen == NULL) {
        perror("Memory allocation failed");
        return 0;
    }

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

            // sprintf(aux, "%d", i-1);
            // printf("%-*s", csv->columnsCount, i == 0? " " : aux);
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

                // sprintf(aux, "%d", i-1);                 
                // printf("%-*s", csv->columnsCount, i == 0 ? " " : aux);
                printf("%-*d", columnsCount, index[i]);

                for (int j = 0; j < columnsCount; j++)
                    printf("\t%s", f[j]);
                printf("\n");
            }
        }

        free(maxStrlen); 
        for (int i = 0; i < columnsCount; i++) free(f[i]);
    }

    printf("[%d rows] x [%d columns]", lineCount-1, columnsCount);
    return 1;
}

int filterFile( csv_t* csv, int index, char* value, int (*func)(char* a, char* b)){

    char ***aux = (char***) malloc(csv->lineCount * sizeof(char**));
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
        aux[0][j] = csv->headerNames[j];
    }

    for (unsigned int i = 1; i < csv->lineCount; i++){
        if (func(csv->matrix[i][index], value)) {

            aux[newLineCount] = csv->matrix[i];
            auxIndex[newLineCount] = csv->index[i];
            for (unsigned int j = 0; j < csv->columnsCount; j++){
                aux[newLineCount][j] = csv->matrix[i][j];
            }
            newLineCount++;
        }
    }

    showFile(aux, auxIndex, newLineCount, csv->columnsCount);

    return 1;
}

void filterEntry( csv_t* csv ){
    char ctrl[3 * sizeof(char)];
    char column[STRING_BUFFER];
    char value[STRING_BUFFER];
    unsigned int i = 0;

    printf("\nEntre com a variável: ");
    fgets(column, STRING_BUFFER, stdin);
    getchar();

    //Remove o /n
    column[strlen(column) - 1] = '\0';

    while (i < csv->columnsCount && eq(csv->headerNames[i], column) != 1) i++;

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

    if (eq(ctrl, "==")) filterFile( csv, i, value, eq);
    else if(eq(ctrl, "!=")) filterFile( csv, i, value, neq);
    else if (eq(ctrl, ">=")) filterFile( csv, i, value, egt);
    else if (eq(ctrl, ">")) filterFile( csv, i, value, gt);
    else if (eq(ctrl, "<=")) filterFile( csv, i, value, elt);
    else if (eq(ctrl, "<")) filterFile( csv, i, value, lt);
}


void fileSummary( csv_t* csv ){
    for (unsigned int i = 0; i < csv->columnsCount; i++){
        printf("\n%s ", csv->headerNames[i]);
        printf("%s", csv->headerTypes[i]);
    }
    printf("\n%d variaveis encontradas", csv->columnsCount);
    printf("\n");
}

void freeMatrix( char*** matrix, unsigned int columnsCount, unsigned int lineCount ){
    for (unsigned int i = 0; i < lineCount; i++){
        for (unsigned int j = 0; j < columnsCount; j++){
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
};

void freeHeader( char** types, char** names, unsigned int columnsCount ){
    for (unsigned int i = 0; i < columnsCount; i++){
        free(types[i]);
        free(names[i]);
    }
    free(types);
    free(names);
}

void freeCSV( csv_t* csv ){
    freeMatrix(csv->matrix, csv->columnsCount, csv->lineCount);
    freeHeader(csv->headerTypes, csv->headerNames, csv->columnsCount);
    free(csv->index);
    fclose(csv->csv_file);
    free(csv);
}
