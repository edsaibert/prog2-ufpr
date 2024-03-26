
#include "io.h"

int main(int argc, char** argv){
    char ctrl;

    if ((argc < 2) || (argc > 2)){
        perror("Path inválido. Execução: ./csvreader.c <path.csv>");
        return 1;
    }

    char path[STRING_BUFFER];
    for (int i = 0; i < argc; i++){
       strcpy(path, argv[i]); 
    }

    do {

        printf("1)\t Sumário do Arquivo\n2)\t Mostrar\n3)\t Fim\n");
        printf("Digite uma opção: ");
        ctrl = getchar();
        printf("\n");

        readFile(path, ctrl - '0');

    } while (ctrl != '3');

    return 0;
}