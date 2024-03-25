#include "io.h"

int readColumns(FILE* csv, char columns[COLUMN_BUFFER]){
    int columnsCount = 0;
    char *p;

    fgets(columns, COLUMN_BUFFER, csv);

    while (strcmp(p, "\0") != 0) {
        sscanf(columns, "%s", p);
        printf("%s", p);
        columnsCount++;
    }

    printf("%s", columns);
    printf("%d", columnsCount);



    return 1;
}

int readFile( char path[STRING_BUFFER] ){
    FILE *csv;
    char columns[COLUMN_BUFFER];

    csv = fopen(path, "r+");

    if (csv == NULL) {
        perror("Arquivo incorreto ou não existente.");
        return 0;
    }

    readColumns(csv, columns);

    return 1;
};