#include "stringItem.h"

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