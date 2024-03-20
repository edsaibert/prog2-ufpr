#include <stdio.h>
#include <stdlib.h>


typedef struct person {
    char id[11];
    int hacking;
    int criminal;
} Person_t;

typedef struct population {
    Person_t* person;
    struct population* next;
} Population_t;

void iniPopulation(Population_t* p){
    p = NULL;
}

void iniPerson(Person_t* p){
    p = NULL;
}

Population_t* readFile( char* file_name ){
     Population_t* head;
     Person_t* person;
     FILE* file;

     iniPopulation(head);
     iniPerson(person);

     file = fopen(file_name, "r+");
     if (!file)
         return NULL;

     while (!feof(file))
     {
        person = (Person_t *)malloc(sizeof(Person_t));
        if (!person)
            return NULL;

        fread(person, sizeof(Person_t), 1, file);

        if (head == NULL){
            head = (Population_t *)malloc(sizeof(Population_t));
            head->person = person;
            head->next = NULL;
        } 
        else {
            Population_t* aux;
            aux = (Population_t *)malloc(sizeof(Population_t));
            aux->person = person;
            aux->next = head;
            head = aux;
        }
     }

    fclose(file);
    return head;
}

void printInformation( Population_t* head ){
    Population_t* temp = head;
    Person_t* person;

    int populationCount = 0;
    int criminals = 0;
    int hacking = 0;
    float habilityMean;

    while (temp) {
        person = temp->person;

        printf(" ----------------------------------------- \n");
        printf("Pessoa: %s\n", person->id);
        printf("Habilidade: %d\n", person->hacking);

        populationCount++;
        hacking = hacking + (person->hacking);
        if (person->criminal > 5)
            criminals++;
       
        temp = temp->next;
    }
    habilityMean = hacking/populationCount;

    printf("\n");
    printf(" Contagem da população: %d\n", populationCount);
    printf(" Média da habilidade: %f\n", habilityMean);
    printf(" Criminais: %d", criminals);
}

int freePopulation(Population_t* head){
    Population_t* aux = head;

    while (head) {
        aux = head->next;
        free(head->person);
        free(head);
        head = aux;
    }
}


int main() {
    Population_t* head;

    head = readFile("citizens.bin");
    printInformation(head);
    freePopulation(head);

    return 0;
}