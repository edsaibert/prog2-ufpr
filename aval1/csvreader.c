
#include "io.h"

int main(int argc, char** argv){

    if ((argc < 2) || (argc > 2)){
        printf("Path inválido. Execução: ./csvreader.c <path.csv>");
        return 1;
    }

    char path[STRING_BUFFER];
    for (int i = 0; i < argc; i++){
       strcpy(path, argv[i]); 
    }
    readFile(path);

    return 0;
}