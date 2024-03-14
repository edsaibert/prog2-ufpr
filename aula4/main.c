#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BITSTREAM 1000 

// Estrutura de dicionário
typedef struct node {
    char c;
    int freq;
    struct node* left;
    struct node* right;
} Node;

int main() {
    FILE* f; // Arquivo (file)
    Node* dict; // Dicionário

    char s[MAX_BITSTREAM];
    char cp[MAX_BITSTREAM];

    f = fopen("robinWood.txt", "r+");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    fgets(s, MAX_BITSTREAM, f);
    printf("%s", s);

    /*

        Criar struct de lista e implementar um dicionario com base
        nas frequencias de cada palavra! 


    */

    fclose(f);
    return 0;
}
