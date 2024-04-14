
#include "io.h"

int main(int argc, char** argv){
    char ctrl;
    csv_t *csv;


    if ((argc < 2) || (argc > 2)){
        perror("Path inválido. Execução: ./csvreader.c <path.csv>");
        return 1;
    }

    char path[STRING_BUFFER];
    for (int i = 0; i < argc; i++){
       strcpy(path, argv[i]); 
    }

    if (!(csv = inicializeCSV(path))) return 1;
    if (!(readCSV(csv))) return 1;

    do {

        printf("\n1)\t Sumário do Arquivo\n" );
        printf("2)\t Mostrar\n");
        printf("3)\t Filtros\n");
        printf("4)\t Descrição dos Dados\n");
        printf("5)\t Ordenação\n");
        printf("6)\t Seleção\n");
        printf("7)\t Dados Faltantes\n");
        printf("8)\t Salvar Dados\n");
        printf("9)\t Sair\n");

        printf("Digite uma opção: ");
        ctrl = getchar();

        /*  Joga fora o char '\n' */
        getchar();
        printf("\n");

        switch(ctrl){
            case '1':
                fileSummary(csv);
                break;
            case '2':
                showFile(csv->matrix, csv->index, csv->lineCount, csv->columnsCount);
                break;
            case '3':
                filterEntry(csv);
                break;
            case '4':
                stat(csv);
                break;
            case '5':
                sortFile(csv);
                break;
            case '9':
                break;
            default:
                perror("Opção não válida.");
                break;
        };

        printf("\nPrecione ENTER para continuar\n");
        getchar();
        // readFile(path, ctrl - '0');

    } while (ctrl != '9');

    freeCSV(csv);

    return 0;
}