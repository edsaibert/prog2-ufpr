#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BITSTREAM 1000

#define eq(a, b) (strcmp(a, b) == 0)
#define lt(a, b) (strcmp(a, b) < 0)
#define gt(a, b) (strcmp(a, b) > 0)
#define cp(a, b) strcpy(a, b)
#define read(a) scanf("%s", a)
#define write(a) printf("%s\n", a)

/*  Definição do tipo contido no item da árvore */
typedef char itemArv[MAX_BITSTREAM];

/*  Estrutura do dicionário */
typedef struct arvBin *apBin;
typedef struct arvBin {
    itemArv item;
    apBin esq, dir;
    int chave;
} arvBin;

typedef apBin arvBusca;


/*  Funções de manipulação da árvore */
void criaDic ( arvBusca* arv ){
    arv = NULL;
}

int arvVazia ( arvBusca arv ){
    return (arv == NULL);
}

arvBusca criaNodo( itemArv item ){
    arvBusca nodo;

    nodo = (arvBusca) malloc(sizeof(arvBin));
    if (arvVazia(nodo))
        return NULL;

    cp(nodo->item, item);
    nodo->chave = 1;
    nodo->esq = NULL;
    nodo->dir = NULL;

    return nodo;
}


arvBusca insereDic ( itemArv item, arvBusca arv ){

    if (arvVazia(arv)){
        return criaNodo(item);
    }
    // arv->chave++;

    if (lt(arv->item, item))
        arv->esq = insereDic(item, arv->esq); 
    else if (gt(arv->item, item))
        arv->dir = insereDic(item, arv->dir);

    return arv;
}

arvBusca updateDic( itemArv item, arvBusca arv ){

    if (eq((arv)->item, item)){
        (arv)->chave++;
        // *arv = aux;

        return arv;
    } 

    if (lt((arv)->item, item))
        arv->esq = updateDic(item, (arv->esq));
    else
        arv->dir = updateDic(item, (arv->dir));
    
    return arv;
}

int max(int a, int b){
    return (a > b) ? a : b;
}

int analyzeDic( arvBusca arv, int freq ){
    if (arvVazia(arv)){
        return freq;
    }

    int left = analyzeDic(arv->esq, freq);
    int right = analyzeDic(arv->dir, freq);

    if (arv->chave > freq){
        freq = arv->chave;
    }
    
    printf("Chave: %d\n", arv->chave);
    return max(freq, max(left, right));
}

void writeDic( arvBusca arv ){
    if (arvVazia(arv))
        return;

    writeDic(arv->esq);
    write(arv->item);
    writeDic(arv->dir);
}


void freeArv ( arvBusca arv ){
    if (arv == NULL)
        return;

    freeArv( arv->esq );
    freeArv( arv->dir );
    free(arv);
}

int main() {
    FILE* f; // Arquivo (file)
    arvBusca dict; // Dicionário

    criaDic(&dict);

    itemArv s;

    f = fopen("robinWood.txt", "r+");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    // fgets(s, MAX_BITSTREAM, f);
    fscanf(f, "%s", s);
    while (!feof(f)) {
        dict = insereDic(s, dict); 
        dict = updateDic(s, dict);

        fscanf(f, "%s", s);
    }

    int freq = analyzeDic(dict, 0); // Como retornar string junto com a frequencia?
    printf("Frequencia: %d\n", freq); 

    fclose(f);
    return 0;
}
